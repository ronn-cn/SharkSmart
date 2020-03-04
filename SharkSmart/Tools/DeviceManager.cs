using HIDAPI;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace SharkSmart
{
    public class DeviceManager
    {
        private readonly HIDInterface hidParent = new HIDInterface();
        private List<HIDDevice> HIDDevices = new List<HIDDevice>();
        public List<SharkDevice> Devices { get; set; }

        public HIDInterface ActivedHID { get; set; }

        private bool isAutoConnect = false;
        public bool IsAutoConnect
        {
            get { return isAutoConnect; }
            set
            {
                isAutoConnect = value;
            }
        }

        public event EventHandler CheckoutDeviceEvent;

        public void AutoRefreshDevice()
        {
            Task.Run(() =>
            {
                while (IsAutoConnect)
                {
                    CheckoutDevice();//检测设备
                    Thread.Sleep(1000);
                }
            });
        }

        /// <summary>
        /// SharkSmart重新连接设备
        /// </summary>
        public bool RestartConnect(HIDInterface hid)
        {
            if (hid == null) return false;
            if (hid.Connected)
            {
                hid.DisConnect();
            }
            return hid.Connect();
        }

        public void ClearVersion(HIDInterface hid)
        {
            if (hid == null) return;
            if (!hid.Connected) hid.Connect();
            hid.Send(CommandHelper.ClearVersion(), true);
        }

        public void Recovery(HIDInterface hid)
        {
            if (hid == null) return;
            if (!hid.Connected) hid.Connect();
            hid.Send(CommandHelper.Recovery(), true);
            Thread.Sleep(500);
            hid.Send(CommandHelper.Restart(), true);
        }


        public DeviceManager()
        {
            this.Devices = new List<SharkDevice>();
            IsAutoConnect = true;
            this.AutoRefreshDevice();
        }

        /// <summary>
        /// 检测设备
        /// </summary>
        public void CheckoutDevice()
        {
            this.HIDDevices.Clear();
            hidParent.GetHidDeviceList(ref HIDDevices, 0x0483, 0x5710);//获取该vidpid的设备列
            List<SharkDevice> sdevs = new List<SharkDevice>();
            foreach(SharkDevice sdev in Devices)
            {
                foreach(HIDDevice hdev in HIDDevices)
                {
                    if (sdev.Device.Compare(hdev))
                    {
                        sdevs.Add(sdev);
                    }
                }
            }
            foreach (HIDDevice hdev in HIDDevices)
            {
                if (hdev.serial.Length != 24) continue;//序列号不等于24位的跳出;
                bool isflag = true;
                foreach (SharkDevice sdev in Devices)
                {
                    if (sdev.Device.Compare(hdev))
                    {
                        isflag = false;
                    }
                }
                if (isflag)
                {
                    sdevs.Add(new SharkDevice(hdev));
                }
            }
            this.Devices = sdevs;
            CheckoutDeviceEvent?.Invoke(this, new EventArgs());
        }
    }
}
