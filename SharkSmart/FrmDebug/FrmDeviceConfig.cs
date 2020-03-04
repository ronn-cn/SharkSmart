using EVTechnology.Common.Controls;
using EVTechnology.Common.Helper;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SharkSmart
{
    public partial class FrmDeviceConfig : EVForm
    {
        public FrmDeviceConfig()
        {
            InitializeComponent();
        }

        private void FrmDeviceConfig_Load(object sender, EventArgs e)
        {
            FormHelper.NumberTextBox(textBox3);
            FormHelper.NumberTextBox(textBox7, 40);
            FormHelper.NumberTextBox(textBox8, 99);
            FormHelper.NumberTextBox(textBox9, 999);
            FormHelper.NumberTextBox(textBox10, 999);
        }

        private void btnHelper_Click(object sender, EventArgs e)
        {

        }

        private void BtnOK_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.OK;
            this.Close();
        }

        private void BtnCancel_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            this.Close();
        }
    }
}
