using EVClassLib;
using System;
using System.Collections.Generic;
using System.Windows.Forms;
using EVTechnology.Logging;
using System.Threading.Tasks;

namespace SharkSmart
{
    public partial class CtlDebug : UserControl
    {
        private readonly DeviceManager devicemanager;
        private List<DeviceItem> items = new List<DeviceItem>();
        private CtlDebugOutput output;

        //开一个任务刷新界面，实时显示当前的设备列表
        public CtlDebug()
        {
            InitializeComponent();
            devicemanager = new DeviceManager();
            devicemanager.CheckoutDeviceEvent += Devicemanager_CheckoutDeviceEvent;
            this.output = new CtlDebugOutput();
        }

        private void Devicemanager_CheckoutDeviceEvent(object sender, EventArgs e)
        {
            if (!this.Created) return;
            this.BeginInvoke(new Action(() =>
            {
                UpdateDeviceList();
            }));
        }

        private void CtlDebug_Load(object sender, EventArgs e)
        {

        }

        private readonly List<DeviceItem> Devices = new List<DeviceItem>();

        private void UpdateDeviceList()
        {
            List<DeviceItem> removeitems = new List<DeviceItem>();
            foreach (DeviceItem item in items)
            {
                if (!devicemanager.Devices.Contains(item.Sdev))
                {
                    removeitems.Add(item);
                    Control[] ctrls = this.panel1.Controls.Find(item.Sdev.Device.serial, false);
                    foreach (Control c in ctrls)
                    {
                        this.panel1.Controls.Remove(c);
                    }
                }
                //item.UpdateCombox();
            }
            foreach (DeviceItem item in removeitems)
            {
                this.items.Remove(item);
            }

            foreach (SharkDevice sdev in devicemanager.Devices)
            {
                DeviceItem item = items.Find(p => p.Sdev == sdev);
                if (item == null)
                {
                    item = new DeviceItem(sdev);
                    items.Add(item);
                    CtlDeviceItem ctl = new CtlDeviceItem(item);
                    ctl.Name = item.Sdev.Device.serial;
                    ctl.Dock = DockStyle.Top;
                    ctl.outputPanel = this.panel2;
                    this.panel1.Controls.Add(ctl);
                    Devices.Add(item);
                }
            }
        }

        private void TsbHelp_Click(object sender, EventArgs e)
        {
            FrmDeviceConfig frm = new FrmDeviceConfig();
            frm.ShowDialog();

            bool flag;
            ToolStripMenuItem ts = sender as ToolStripMenuItem;
            if (ts.Text == "进入救援模式") flag = true;
            else flag = false;


            CtlAdvancedDebug ctl = new CtlAdvancedDebug();
            ctl.Devices = Devices;
            ctl.Text = flag ? "救援" : "清除";
            if (ctl.ShowDialog() == DialogResult.OK)
            {
                DeviceItem item = ctl.Item;
                if (item != null)
                {
                    SharkDevice sd = devicemanager.Devices.Find(p => p.Device.serial == ctl.Item.Sdev.Device.serial);
                    if (flag)
                        devicemanager.Recovery(sd);
                    else
                        devicemanager.ClearVersion(sd);
                }
            }
        }

        private void CbCheck_CheckedChanged(object sender, EventArgs e)
        {
            foreach (var ctl in panel1.Controls)
            {
                if (ctl is CtlDeviceItem)
                {
                    ((CtlDeviceItem)ctl).cbCheck.Checked = cbCheck.Checked;
                }
            }
        }

        private void TsbRefresh_Click(object sender, EventArgs e)
        {
            UpdateDeviceList();
            foreach (var ctl in panel1.Controls)
            {
                var tmp = ctl as CtlDeviceItem;
                tmp.UpdateCombox();
            }
        }

        private void TsbStart_Click(object sender, EventArgs e)
        {
            foreach (var it in items)
            {
                it?.Run();
            }
        }

        private void TsbCompile_Click(object sender, EventArgs e)
        {
            //编译不烧录
            CompileHelper compiler = new CompileHelper();
            compiler.Logger = new EVTechnology.Logging.Logger();
            this.output.SetLogger(compiler.Logger);

            if (this.panel2 != null)
            {
                this.output.Dock = DockStyle.Fill;
                this.panel2.Controls.Clear();
                this.panel2.Controls.Add(this.output);
            }
            Task.Run(() =>
            {
                List<ExecModule> mods = Engine.Project.GetAllModule();
                foreach (var mod in mods)
                {
                    compiler.SelectedModule = mod;
                    compiler.Compile();
                }
            });
        }
    }
}
