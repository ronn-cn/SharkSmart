using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using HIDAPI;
using EVTechnology.Common.Helper;
using System.IO;

namespace SharkSmart
{
	public class SharkDevice : HIDInterface 
	{
		private DiskDriveInfo driveinfo;

        public string DrivePath { get { return driveinfo?.DriveLetter; } }

		public SharkDevice():base()
		{

		}

		public SharkDevice(HIDDevice hdev):base(hdev)
		{
			  this.driveinfo = USB.WhoDiskDevice(this.Device.vID, this.Device.pID, this.Device.serial);
		}
		
		public void Upload(string sourcepath, string newpath, bool isoverwrite)
		{
			if(!Directory.Exists(Path.GetDirectoryName(newpath)))
			{
				Directory.CreateDirectory(Path.GetDirectoryName(newpath));
			}
			File.Copy(sourcepath, newpath, isoverwrite);
		}

		public void Upload(string sourcepath, string newpath)
		{
			if (!Directory.Exists(Path.GetDirectoryName(newpath)))
			{
				Directory.CreateDirectory(Path.GetDirectoryName(newpath));
			}
			File.Copy(sourcepath, newpath, true);
		}

		/// <summary>
		/// 更新文件到模块的根目录中
		/// </summary>
		public bool Upload(string sourcepath)
		{
			string newpath = "";
			if(driveinfo != null)
			{
				newpath = driveinfo.DriveLetter + "\\" + Path.GetFileName(sourcepath);
				if(Directory.Exists(driveinfo.DriveLetter))
				{
					File.Copy(sourcepath, newpath, true);
				}
				if(File.Exists(newpath))
				{
					return true;
				}
			}
			return false;
		}

		/// <summary>
		/// 更新文件夹，将文件夹内的文件全部上传到模块中
		/// </summary>
		public bool UploadDirectory(string sourcepath)
		{
			string newpath = "";
			if (driveinfo != null)
			{
				newpath = driveinfo.DriveLetter + "\\";
				if (Directory.Exists(driveinfo.DriveLetter))
				{
					FileHelper.CopyDirectory(sourcepath, newpath);
				}
				if (File.Exists(newpath+"app.bin"))
				{
					return true;
				}
			}
			return false;
		}
    }
}
