using EVClassLib;
using EVTechnology.Helper;
using EVTechnology.Logging;
using IronPython.Compiler.Ast;
using SharkCompiler;
using Microsoft.Scripting;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Security.Cryptography;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SharkSmart
{
	public delegate void ProgressHandler(int value);
	/// <summary>
	/// 编译助手
	/// </summary>
	public class CompileHelper
	{
		/// <summary>
		/// 选择的模块,必须选择,若不选择则不能编译
		/// </summary>
		public ExecModule SelectedModule { get; set; }

		private int _progress = 0;
		public int Progress { get { return _progress; } }

		/// <summary>
		/// 该编译助手的日志
		/// </summary>
		public Logger Logger { get; set; }

		public event ProgressHandler ProgressChangeEvent;
		public event EventHandler IsRunningChanged;

		private bool isrunning = false;
		/// <summary>
		/// 编译助手是否正在运行
		/// </summary>
		public bool IsRunning
		{
			get { return isrunning; }
			set
			{
				if (value != isrunning)
				{
					isrunning = value;
					if (IsRunningChanged != null)
						IsRunningChanged(this, new EventArgs());
				}
			}
		}
		/// <summary>
		/// 目标文件是否编译OK,目标文件app.bin
		/// </summary>
		public bool IsTargetOk { get; set; } = false;

		public CompileHelper()
		{
			options = new Dictionary<string, string>();
			translator = new Translator();
		}

		private Process pMake;
		private CancellationTokenSource cts;
		private string code_global_user = "";
		private Dictionary<string, string> options;//翻译器需要的kv值项
		private Translator translator;//编译助手需要的翻译器

		public void Compile()
		{
			this.IsTargetOk = false;
			this.IsRunning = true;
			//先进行保存操作,将整个项目的内容保存,在进行编译
			Engine.Project.Save();
			if (SelectedModule == null) return;
			if (this.Logger != null)
				this.Logger.OutputStreamEvent += new OutputStreamHandler(LoggerOutputStream);
			string output_path = Engine.Project.WorkPath + "\\" + SelectedModule.Name + SelectedModule.OutputPath;
			string app_bin_path = output_path + "\\Bin\\app.bin";
			string makefile_path_path = output_path + "\\Makefile_Path";
			try
			{
				//-1. 选择mod
				options.Clear();
				if (!Directory.Exists(output_path))
				{
					Directory.CreateDirectory(output_path);
				}
				//将Project文件夹拷贝过来
				string path = Path.Combine(Environment.CurrentDirectory, SoftwareSetting.SourceCodePath, SelectedModule.PresetName, "Project");
				FileHelper.CopyDirectory(path, output_path);
				Getdir(path, ".c");
				SetProgressValue(5);

				//1.py -> c
				//one. 提取全局变量
				foreach (Unit unit in SelectedModule.Units)
				{
					translator.TransformGlobalData(GetFilePath("Units\\" + unit.Name));
					options.Add(unit.Name, unit.Type.ToString());
					if (unit.GetType() == typeof(UIUnit))
					{
						UIUnit ui = unit as UIUnit;
						foreach (ViewPage page in ui.Pages)
						{
							PythonGlobalData pygdata0 = new PythonGlobalData(page.Name);
							translator.GlobalDatas.Add(pygdata0);
							options.Add(page.Name, page.Type.ToString());
							foreach (SControl ctrl in page.Controls)
							{
								if (!options.ContainsKey(ctrl.Name))
								{
									pygdata0.Names.Add(new NameExpression(ctrl.Name));
									options.Add(ctrl.Name, ctrl.Type.ToString());
								}
							}
						}
					}
				}
				translator.Options = options;

				foreach (Plan plan in SelectedModule.Plans)
				{
					translator.TransformGlobalData(GetFilePath("Plans\\" + plan.Name));
				}

				PythonGlobalData pygdata = new PythonGlobalData("Data");
				foreach (Resource res in SelectedModule.Resources)
				{
					SelectedModule.Data.Datas.Add(res.ToData());
				}

				foreach (Data d in SelectedModule.Data.Datas)
				{
					pygdata.Names.Add(new NameExpression(d.Key));
				}
				translator.GlobalDatas.Add(pygdata);

				int fileCount = SelectedModule.GetFileCount() + 2;
				int index = 0;

				string code_unit = "";
				//two. 生成文件
				foreach (Unit unit in SelectedModule.Units)
				{
					unit.Data = translator.TransformPath(GetFilePath("Units\\" + unit.Name));
					string unitcodec = unit.TransformToClang_forC();
					File.WriteAllText(output_path + "\\User\\" + unit.Name + ".c", unitcodec);

					SetProgressValue(index++ * 20 / fileCount);

					code_global_user += unit.TransformToClang_forH();
					code_unit += unit.Name + "_Init();\r\n";
					if (unit.GetType() == typeof(UIUnit))
					{
						UIUnit ui = unit as UIUnit;
						foreach (ViewPage page in ui.Pages)
						{
							//page.Data = translator.TransformPath(GetFilePath("Units\\Pages\\" + page.Name));
							string pagecode = page.TransformToClang_forC();
							File.WriteAllText(output_path + "\\User\\" + page.Name + ".c", pagecode);
							code_global_user += page.TransformToClang_forH();

							SetProgressValue(index++ * 20 / fileCount);
						}
					}

				}

				string code_unit_c = @"#include ""GLOBAL.h""

void UnitRegister()
{
" + code_unit + @"
	ETH_Init();
}
";
				File.WriteAllText(output_path + "\\User\\Unit.c", code_unit_c);
				SetProgressValue(index++ * 20 / fileCount);

				string code_task = "";
				foreach (Plan plan in SelectedModule.Plans)
				{
					plan.Data = translator.TransformPath(GetFilePath("Plans\\" + plan.Name));
					string plancode = plan.TransformToClang_forC();

					File.WriteAllText(output_path + "\\User\\" + plan.Name + ".c", plancode);
					code_global_user += plan.TransformToClang_forH();
					code_task += plan.Name + "_Start();\r\n";

					SetProgressValue(index++ * 20 / fileCount);
				}
				string code_task_c = @"#include ""GLOBAL.h""

void TaskRegister()
{
" + code_task + @"
}
";
				File.WriteAllText(output_path + "\\User\\Task.c", code_task_c);

				string datacode = SelectedModule.Data.TransformToClang_forC();
				File.WriteAllText(output_path + "\\User\\Data.c", datacode);
				code_global_user += SelectedModule.Data.TransformToClang_forH();
				string code_global_user_h = @"#ifndef __GLOBAL_USER_H
#define __GLOBAL_USER_H

" + code_global_user + @"

#endif";

				File.WriteAllText(output_path + "\\User\\GLOBAL_USER.h", code_global_user_h);
				SetProgressValue(index++ * 20 / fileCount);
				if (File.Exists(makefile_path_path))
				{
					string[] makefile_arr = File.ReadAllLines(makefile_path_path, Encoding.UTF8);

					makefile_arr[0] = "ROOT_DIR = " + Environment.CurrentDirectory;
					makefile_arr[1] = "LIB_DIR = $(ROOT_DIR)\\Library\\Components\\" + SelectedModule.PresetName;
					File.WriteAllLines(makefile_path_path, makefile_arr);
				}
			}
			catch (Exception ex)
			{
				Console.WriteLine(ex.Message);
				this.Logger.Error("{0}-{1}", "PythonToC", ex.Message);
				this.IsRunning = false;
			}

			//2.C -> bin
			if (!IsRunning)
			{
				this.Logger?.Wran("{0}-{1}", "中断", "编译任务已中断!");
				SetProgressValue(100);
				return;
			}
			cts = new CancellationTokenSource();

			pMake = CreateProcess(Environment.CurrentDirectory + "\\make.exe", "-C", output_path);
			if (pMake == null) return;

			pMake.OutputDataReceived += new DataReceivedEventHandler(P_OutputDataReceived);
			pMake.ErrorDataReceived += new DataReceivedEventHandler(P_ErrorDataReceived);
			Task t = Task.Run(() =>
			{
				pMake.Start();
				pMake.BeginOutputReadLine();
				pMake.BeginErrorReadLine();
				pMake.WaitForExit();
				if (cts.Token.IsCancellationRequested)//如果检测到取消请求
				{
					this.Logger?.Wran("{0}-{1}", "GCC编译", "任务被终止！");
					SetProgressValue(100);
				}
				pMake.CancelOutputRead();
				pMake.CancelErrorRead();
				pMake.Close();
				pMake.Dispose();
				if (File.Exists(app_bin_path))
				{
					object value = new object();
					int[] version;
					bool one = SelectedModule.Setting.TryGetValue("version", out value);
					if (one && value.GetType() == typeof(int[]))
					{
						version = (int[])value;
					}
					else
					{
						version = new int[4];
						version[0] = 0;
						version[1] = 0;
						version[2] = 0;
						version[3] = 1;
					}
					this.AddVersion(app_bin_path, version);
					//文件拷贝到upload文件夹
					string upload_path = output_path + "\\upload";
					FileHelper.DeleteFolder(upload_path);
					if (!Directory.Exists(upload_path))
					{
						Directory.CreateDirectory(upload_path);
					}
					Directory.CreateDirectory(upload_path + "\\resources");
					File.Copy(app_bin_path, upload_path + "\\" + Path.GetFileName(app_bin_path), true);
					FileHelper.CopyDirectory(Engine.Project.WorkPath + "\\" + this.SelectedModule.Name + "\\Resources", upload_path + "\\resources");
				}
				else
				{
					this.Logger.Error("{0}-{1}", "GCC编译", "编译生成失败！");
					SetProgressValue(100);
				}
				IsRunning = false;
			}, cts.Token);
			t.Wait();
		}


		/// <summary>
		/// 停止编译
		/// </summary>
		public void StopCompile()
		{
			if (cts != null)
			{
				cts.Cancel();
			}
			KillProcess("make");
			this.IsRunning = false;
		}

		private int index2 = 0;
		private void LoggerOutputStream(string text)
		{
			if (text.Contains(".o"))
			{
				SetProgressValue(index2++ * 80 / SourFileCount + 20);
			}
			else if (text.Contains("app.bin"))
				SetProgressValue(100);
		}

		private void SetProgressValue(int value)
		{
			if (ProgressChangeEvent != null)
				ProgressChangeEvent(value);
		}

		/// <summary>
		/// 创建线程并返回该线程
		/// </summary>
		/// <param name="filename">程序路径</param>
		/// <param name="args">参数集合</param>
		/// <returns></returns>
		private Process CreateProcess(string filename, params string[] args)
		{
			try
			{
				string s = "";
				foreach (string arg in args)
				{
					s += arg + " ";
				}
				Process myprocess = new Process();
				ProcessStartInfo startInfo = new ProcessStartInfo(filename, s.Trim());
				myprocess.StartInfo = startInfo;

				//通过以下参数可以控制exe的启动方式
				myprocess.StartInfo.CreateNoWindow = true;
				myprocess.StartInfo.UseShellExecute = false;
				myprocess.StartInfo.RedirectStandardInput = true;
				myprocess.StartInfo.RedirectStandardOutput = true;
				myprocess.StartInfo.RedirectStandardError = true;
				return myprocess;
			}
			catch (Exception ex)
			{
				if (this.Logger != null)
				{
					this.Logger.Error("{0}-{1}", "线程", "启动应用程序时出错！原因：" + ex.Message);
				}
				MessageBox.Show("启动应用程序时出错！原因：" + ex.Message);
			}
			return null;
		}

		/// <summary>
		/// 结束线程
		/// </summary>
		/// <param name="processName"></param>
		private void KillProcess(string processName)
		{
			Process[] myproc = Process.GetProcesses();
			foreach (Process item in myproc)
			{
				if (item.ProcessName == processName)
				{
					item.Kill();
				}
			}
		}


		private void P_OutputDataReceived(object sender, DataReceivedEventArgs e)
		{
			if (!string.IsNullOrEmpty(e.Data))
			{
				if (this.Logger != null)
				{
					this.Logger.Debug("{0}-{1}", "编译", e.Data);
				}
			}
		}

		private void P_ErrorDataReceived(object sender, DataReceivedEventArgs e)
		{
			if (!string.IsNullOrEmpty(e.Data))
			{
				if (this.Logger != null)
				{
					this.Logger.Error("{0}-{1}", "编译", e.Data);
				}
				if (e.Data.Contains("error") || e.Data.Contains("Error"))
				{
					//IsRunning = false;
					if (cts != null)
					{
						cts.Cancel();
					}
				}
			}
		}


		/// <summary>
		/// 获取文件路径，将模块路径与名字链接
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		private string GetFilePath(string name)
		{
			return Engine.Project.WorkPath + "\\" + SelectedModule.Name + "\\" + name + ".py";
		}

		/// <summary>
		/// 递归获取指定类型文件
		/// </summary>
		private int SourFileCount = 0;
		private void Getdir(string path, string extName)
		{
			string[] dir = Directory.GetDirectories(path); //文件夹列表   
			DirectoryInfo fdir = new DirectoryInfo(path);
			FileInfo[] file = fdir.GetFiles();
			//FileInfo[] file = Directory.GetFiles(path); //文件列表   
			if (file.Length != 0 || dir.Length != 0) //当前目录文件或文件夹不为空                   
			{
				foreach (FileInfo f in file) //显示当前目录所有文件   
				{
					if (extName.ToLower().IndexOf(f.Extension.ToLower()) >= 0)
					{
						SourFileCount++;
					}
				}
				foreach (string d in dir)
				{
					Getdir(d, extName);//递归   
				}
			}
		}

		/// <summary>
		/// 添加版本号的方法
		/// </summary>
		private void AddVersion(string binpath, int[] vers)
		{
			try
			{
				if (vers != null && vers.Length >= 4)
				{
					byte[] version = new byte[8];
					byte[] hd_version = BitConverter.GetBytes(2);
					Array.Copy(hd_version, version, 4);
					string sw_version_str = vers[0].ToString() + vers[1].ToString()
						+ vers[2].ToString() + (vers[3]).ToString();
					byte[] sw_version = BitConverter.GetBytes(Convert.ToInt32(sw_version_str));
					Array.Copy(sw_version, 0, version, 4, 4);

					byte[] file_bin = File.ReadAllBytes(binpath);
					FileStream fs = new FileStream(binpath, FileMode.Create);
					byte[] ver_file_bin = new byte[file_bin.Length + 8];
					Array.Copy(version, ver_file_bin, 8);
					Array.Copy(file_bin, 0, ver_file_bin, 8, file_bin.Length);
					MD5 md5_0 = new MD5CryptoServiceProvider();
					byte[] output_0 = md5_0.ComputeHash(ver_file_bin);
					BinaryWriter bw = new BinaryWriter(fs);
					bw.Write(output_0);
					bw.Write(ver_file_bin);
					bw.Close();
					fs.Close();
					Console.WriteLine("添加版本号成功！");
					if (this.Logger != null)
					{
						this.Logger.Debug("{0}-{1}", "编译", "添加版本号成功！");
						this.IsTargetOk = true;
					}
				}
				else
				{
					if (this.Logger != null)
					{
						this.Logger.Error("{0}-{1}", "编译", "不是有效的版本号");
					}
				}
			}
			catch (Exception ex)
			{
				if (this.Logger != null)
				{
					this.Logger.Error("{0}-{1}", "编译", "版本号添加过程失败" + ex.ToString());
				}
			}
		}
	}
}
