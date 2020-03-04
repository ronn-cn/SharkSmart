using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Controls;
using EVClassLib;
using EVTechnology.Common.Logging;

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
			Task.Run(()=> {
				Sdev.Connect();
				this.compiler.SelectedModule = Module;
				this.compiler.Compile();
				//上传
				if(this.compiler.IsTargetOk)
				{
					//bool isSuccess = Sdev.Upload(Engine.Project.WorkPath + "\\" + Module.Name + Module.OutputPath + "\\Bin\\app.bin");
					bool isSuccess = Sdev.UploadDirectory(Engine.Project.WorkPath + "\\" + Module.Name + Module.OutputPath + "\\upload");
					if (isSuccess)
					{
						if(Sdev.Connected)
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
			},cts.Token);
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
