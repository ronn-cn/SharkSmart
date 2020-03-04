using EVClassLib;
using HIDAPI;
using System;
using System.Collections.Generic;
using System.Threading;
using System.Windows.Forms;

namespace SharkSmart
{
    public partial class CtlDebug : UserControl
    {
        private DeviceManager devicemanager;
        private List<DeviceItem> items = new List<DeviceItem>();

        //开一个任务刷新界面，实时显示当前的设备列表
        public CtlDebug()
        {
            InitializeComponent();
            devicemanager = new DeviceManager();
            devicemanager.CheckoutDeviceEvent += Devicemanager_CheckoutDeviceEvent;
        }

        private void Devicemanager_CheckoutDeviceEvent(object sender, EventArgs e)
        {
            this.BeginInvoke(new Action(() => {
                UpdateDeviceList();
            }));
        }

        private void CtlDebug_Load(object sender, EventArgs e)
        {

        }

        private List<DeviceItem> Devices = new List<DeviceItem>();

        private void UpdateDeviceList()
        {
            List<DeviceItem> removeitems = new List<DeviceItem>();
            foreach(DeviceItem item in items)
            {
                if(!devicemanager.Devices.Contains(item.Sdev))
                {
                    removeitems.Add(item);
                    Control[] ctrls = this.panel1.Controls.Find(item.Sdev.Device.serial, false);
                    foreach(Control c in ctrls)
                    {
                        this.panel1.Controls.Remove(c);
                    }
                }
            }
            foreach(DeviceItem item in removeitems)
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
            ctl.Text = flag ?"救援" : "清除";
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

        private SharkDevice tmp;

        private void button1_Click(object sender, EventArgs e)
        {
            tmp = new SharkDevice();
            DeviceItem item = new DeviceItem(tmp);
            item.Module = Engine.Project.GetAllModule()[0];
            item.Run();
        }
    }
}
