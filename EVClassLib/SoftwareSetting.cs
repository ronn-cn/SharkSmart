using System;

namespace EVClassLib
{
    public class SoftwareSetting
    {
        //常用设置
        public string CreateDefultPath;             //创建项目的默认路径
        public bool IsSaveHistoryOpenProject;       //是否保存历史打开的项目
        public int SaveHistoryOpenProjectCount;     //保存历史打开项目的数量
        public string FileEncoding;                 //文件编码

        //编辑器设置
        public string EditorFont;                   //字体	
        public int EditorFontSize;                  //字号
        public bool ShowLineNumbers;                //显示行数
        public bool ShowMatchingBracket;            //匹配括号

        //调试设置
        public bool IsUseDebugLog;                  //是否启用调试日志
        public string OutputLogPath;                //日志输出路径
        public int AutoSaveLogFileTimeSpan;         //自动保存日志文件的时间间隔

        //关于软件
        public string SoftWareName = "SharkSmart";
        public string SoftWareVersion = "0.8.0";
        public string GCCVersion = "5.4_2016q3";
        public string CopyRight = "YQKJ";

        public static int MaxRecordCount = 30;      // 保存记录上限条数

        //资源路径
        public static string RecordPath = "Data\\used.field";                       // 记录保存路径(相对于软件的相对路径)
        public static string SourceCodePath = "Library\\Components";                // 源码文件夹路径

        public static string DisplayUnitName = "DisplayUnit";

        public SoftwareSetting()
        {
            InitializeSettingObject();
        }

        public void InitializeSettingObject()
        {
            this.CreateDefultPath = Environment.GetFolderPath(Environment.SpecialFolder.DesktopDirectory);
            this.IsSaveHistoryOpenProject = true;
            this.SaveHistoryOpenProjectCount = 30;
            this.FileEncoding = "UTF-8";

            this.EditorFont = "Consolas";
            this.EditorFontSize = 12;
            this.ShowLineNumbers = true;
            this.ShowMatchingBracket = true;

            this.IsUseDebugLog = true;
            this.OutputLogPath = Environment.CurrentDirectory + @"\Data\Logs\";
            this.AutoSaveLogFileTimeSpan = 10;
        }
    }
}
