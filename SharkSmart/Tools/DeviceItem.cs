using EVClassLib;
using EVTechnology.Common.Helper;
using EVTechnology.Common.Logging;
using System.Collections.Generic;
using System.IO;
using System.Threading;
using System.Threading.Tasks;

namespace SharkSmart
{
	public class DeviceItem
	{
		public SharkDevice Sdev { get; set; }

		public ExecModule Module { get; set; }

		public int Progress { get { return _progress; } }

		private int _progress;
		public Logger Logger { get; } = new Logger();
		public CompileHelper compiler;

		public DeviceItem(SharkDevice device)
		{
			this.Sdev = device;
			this.compiler = new CompileHelper();
			this.compiler.Logger = Logger;
		}

		CancellationTokenSource cts;
		public void Run()
		{
			//开始一个任务
			if (Sdev == null) return;
			if (Module == null) return;
			if (this.compiler.IsRunning) return;
			_progress = 0;
			cts = new CancellationTokenSource();
			Task.Run(() =>
			{
				Sdev.Connect();
				this.compiler.SelectedModule = Module;
				this.compiler.Compile();
				//上传
				if (this.compiler.IsTargetOk)
				{
					//bool isSuccess = Sdev.Upload(Engine.Project.WorkPath + "\\" + Module.Name + Module.OutputPath + "\\Bin\\app.bin");
					string path = Engine.Project.WorkPath + "\\" + Module.Name + Module.OutputPath + "\\upload";

					bool isSuccess = Sdev.UploadDirectory(path);
					SendFileToService(path);
					if (isSuccess)
					{
						if (Sdev.Connected)
						{
							Sdev.Send(CommandHelper.Restart());//发送重启命令
						}
						else
						{
							this.compiler.Logger.Error("{0}-{1}", "连接", "设备连接失败!未能重新启动");
						}
					}
				}
				Thread.Sleep(1000);
				Sdev.DisConnect();
			}, cts.Token);
		}


		/// <summary>
		/// 上传APP和资源文件到服务器
		/// </summary>
		/// <param name="path"></param>
		private void SendFileToService(string path)
		{
			List<string> paths = new List<string>
			{
				path + "\\app.bin"
			};

			List<FileInfo> files = new List<FileInfo>();
			files = FileHelper.GetFileByExt(path, ".raw", files);
			foreach (FileInfo info in files)
			{
				paths.Add(info.FullName);
			}
			string result = HttpFileHelper.SendFileToService("http://172.16.7.11:8000/Upload.php", paths.ToArray());
		}

		public void Stop()
		{
			if (cts != null)
			{
				cts.Cancel();
			}
			this.compiler.StopCompile();
		}
	}
}
