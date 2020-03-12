using EVClassLib;
using NetDimension.NanUI;
using System;
using System.IO;

namespace SharkSmart
{
    public partial class FrmEditor : Formium
    {
        private string filePath;
        public string FilePath
        {
            set
            {
                filePath = value;
                string content = "";
                if (!string.IsNullOrEmpty(filePath))
                    content = File.ReadAllText(filePath).Replace("\n", "\\n").Replace("\r", "\\r");
                ExecuteJavascript("setValue(\"" + content + "\")");

                string modName = Path.GetFileName(Path.GetDirectoryName(Path.GetDirectoryName(filePath)));

                string str = "";

                ExecModule mod = Engine.Project.GetModule(modName) as ExecModule;
                UIUnit ut = mod.GetUIUnit();
                if (ut != null)
                {
                    foreach (var page in ut.Pages)
                    {
                        foreach (var ctl in page.Controls)
                            str += ctl.Name + "|";
                    }
                }
                ExecuteJavascript("setTips(\"" + str + "\")");
            }
            get
            {
                return filePath;
            }
        }

        //public event EventHandler UpdateEvent;

        public FrmEditor() : base(Environment.CurrentDirectory + @"\editor\editor.html")
        {
            InitializeComponent();
            var fun = GlobalObject.AddFunction("SaveFile");
            fun.Execute += Fun_Execute;
        }

        private void Fun_Execute(object sender, Chromium.Remote.Event.CfrV8HandlerExecuteEventArgs e)
        {
            string str = e.Arguments[0].StringValue;
            if (!string.IsNullOrEmpty(FilePath))
                File.WriteAllText(FilePath, str);
        }

        private void FrmEditor_Load(object sender, EventArgs e)
        {
            //LoadHandler.OnLoadEnd += LoadHandler_OnLoadEnd;
            //FilePath = @"C:\Users\admin\Desktop\wechat.py";

        }

        private void LoadHandler_OnLoadEnd(object sender, Chromium.Event.CfxOnLoadEndEventArgs e)
        {

            if (e.Frame.IsMain)
            {
            }
        }

        public void UpdateText()
        {
            string content = "";
            if (!string.IsNullOrEmpty(filePath))
                content = File.ReadAllText(filePath).Replace("\n", "\\n").Replace("\r", "\\r");
            ExecuteJavascript("setValue(\"" + content + "\")");
        }
    }
}
