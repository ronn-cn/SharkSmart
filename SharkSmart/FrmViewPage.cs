using EVClassLib;
using EVTechnology.Common.Controls;
using System;
using System.Drawing;
using System.IO;
using System.Windows.Forms;

namespace SharkSmart
{
    public partial class FrmViewPage : Form
    {
        public string FilePath { set; get; }

        public Size tmp = new Size(800, 480);

        public EVClassLib.Module Mod { set; get; }

        public FrmMain Main { set; get; }

        private EVDesigner deg;

        public FrmViewPage()
        {
            InitializeComponent();
        }

        private void FrmViewPage_Load(object sender, EventArgs e)
        {
            deg = new EVDesigner();
            deg.tsbSave.Click += TsbSave_Click;
            deg.ThemeType = ThemeType.BlackBlue;
            deg.Style = DockStyle.Right;
            deg.Dock = DockStyle.Fill;
            deg.SavePath = FilePath;
            deg.AreaSize = tmp;
            //deg.BlockClickedEvent += Deg_BlockClickedEvent;
            deg.BlockCreatedEvent += Deg_BlockCreatedEvent;
            deg.BlockDoubleClickedEvent += Deg_BlockDoubleClickedEvent;
            Controls.Add(deg);
        }

        private void Deg_BlockDoubleClickedEvent()
        {
            if (deg.CurCvs.Selected != null)
            {
                Unit unit = ((ExecModule)Mod).Units.Find(p => p.GetType() == typeof(UIUnit));
                if (unit == null) return;
                string path = Path.GetDirectoryName(Mod.WorkPath) + "\\Units\\" + unit.Name + ".py";
                string code = File.ReadAllText(path);

                var block = deg.CurCvs.Selected;
                if (string.IsNullOrEmpty(block.OtherFiled))
                {
                    block.OtherFiled = deg.CurCvs.Name + "_" + block.Name + "_Clicked";
                }
                string stra = Path.GetFileNameWithoutExtension(FilePath);
                string rowA = stra + "." + deg.CurCvs.Selected.Name
                    + ".Clicked += " + block.OtherFiled + "\n";

                if (!code.Contains(rowA))
                {
                    string rowb = "def " + block.OtherFiled + "():\n    pass";
                    code += "\n" + rowA + rowb;
                    File.WriteAllText(path, code);
                }
                deg.Save();
                Mod.Save();
                Main.SkipCodeView(path);
            }

        }

        private void Deg_BlockCreatedEvent()
        {
            deg.Save();
            Mod.Save();
        }

        private void TsbSave_Click(object sender, EventArgs e)
        {
            Mod.Save();
        }

        protected override bool ProcessCmdKey(ref Message msg, Keys keyData)
        {
            bool flag = base.ProcessCmdKey(ref msg, keyData);
            if (keyData == (Keys.S | Keys.Control))
            {
                Mod.Save();
            }
            return flag;
        }
    }
}
