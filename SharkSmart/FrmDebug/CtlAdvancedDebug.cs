using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace SharkSmart
{
    public partial class CtlAdvancedDebug : Form
    {
        public List<DeviceItem> Devices = new List<DeviceItem>();

        public DeviceItem Item { set; get; }

        public CtlAdvancedDebug()
        {
            InitializeComponent();
        }

        private void BtnSubmit_Click(object sender, EventArgs e)
        {
            if (cbDevcie.SelectedItem == null) return;
            string name = cbDevcie.SelectedItem.ToString();
            if (string.IsNullOrEmpty(name)) return;
            Item = Devices.Find(p => p.Sdev.Device.pName == name);
            this.DialogResult = DialogResult.OK;
            this.Close();
        }

        private void BtnCancel_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
            Close();
        }

        private void CtlAdvancedDebug_Load(object sender, EventArgs e)
        {
            btnSubmit.Text = Text;
            this.StartPosition = FormStartPosition.CenterScreen;
            cbDevcie.Items.Clear();
            foreach (var device in Devices)
            {
                cbDevcie.Items.Add(device.Sdev.Device.pName);
            }
        }
    }
}
