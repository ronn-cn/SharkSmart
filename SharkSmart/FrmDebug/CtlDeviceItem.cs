using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using EVClassLib;
using System.Diagnostics;

namespace SharkSmart
{
    public partial class CtlDeviceItem : UserControl
    {
        public bool IsRunning { get; set; }

        public CtlDeviceItem()
        {
            InitializeComponent();
        }

        public CtlDeviceItem(DeviceItem item):this()
        {
            this.Item = item;
            this.Output = new CtlDebugOutput(this.Item.Logger);
            this.Item.compiler.ProgressChangeEvent += new ProgressHandler(ValueChange);
            this.Item.compiler.IsRunningChanged += new EventHandler(IsRunningChanged);
        }

        private void ValueChange(int value)
        {
            pbar.Invoke(new Action<int>(SetValue), value);
        }

        private void IsRunningChanged(object sender, EventArgs e)
        {
            this.BeginInvoke(new Action(() =>
            {
                if (this.Item.compiler.IsRunning)
                {
                    btnQucik.BackgroundImage = Properties.Resources.stop;
                    pbar.Visible = true;
                }
                else
                {
                    btnQucik.BackgroundImage = Properties.Resources.play;
                    //pbar.Visible = false;
                }
            }));
        }

        private void SetValue(int value)
        {
            if (value > 100) value = 100;
            if (value == 100)
                btnQucik.BackgroundImage = Properties.Resources.play__2_;
            pbar.Value = value;
        }
        
        public Panel outputPanel;

        /// <summary>
        /// 控件中的设备项
        /// </summary>
        public DeviceItem Item { get; set; }

        private CtlDebugOutput Output;

        private void CtlCompile_Load(object sender, EventArgs e)
        {
            UpdateCombox();
        }

        private void CtlCompile_Paint(object sender, PaintEventArgs e)
        {
            if (Item != null)
            {
                this.lbName.Text = this.Item.Sdev.Device.pName;
            }
            Graphics g = e.Graphics;
            Pen p = new Pen(Color.White, 2.0f);
            g.DrawLine(p, e.ClipRectangle.X, e.ClipRectangle.Y, e.ClipRectangle.X + e.ClipRectangle.Width, e.ClipRectangle.Y);
        }

        private void UpdateCombox()
        {
            cbmod.Items.Clear();
            if (Engine.Project == null) return;
            List<ExecModule> mods = Engine.Project.GetAllModule();
            foreach (var mod in mods)
            {
                cbmod.Items.Add(mod.Name);
            }
        }

        private void BtnQucik_Click(object sender, EventArgs e)
        {
            if (this.Item == null) return;
            if(this.outputPanel != null)
            {
                this.Output.Dock = DockStyle.Fill;
                this.outputPanel.Controls.Clear();
                this.outputPanel.Controls.Add(this.Output);
            }
            this.Item.Run();
        }

        private void Cbmod_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (Engine.Project == null) return;
            List<ExecModule> mods = Engine.Project.GetAllModule();
            this.Item.Module = mods.Find(p => p.Name == (string)this.cbmod.SelectedItem);
        }

        private void btnConnect_Click(object sender, EventArgs e)
        {
            if (this.Item == null) return;
            this.Item.Stop();
        }

        private void BtnConfig_Click(object sender, EventArgs e)
        {
            FrmDeviceConfig frm = new FrmDeviceConfig();
            frm.ShowDialog(); 
        }

        private void BtnFolder_Click(object sender, EventArgs e)
        {
            string path = Item.Sdev.DrivePath;
            if (!string.IsNullOrEmpty(path))
                Process.Start(path);
        }
    }
}
