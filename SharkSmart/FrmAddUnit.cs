using EVClassLib    ;
using EVTechnology.Common.Controls;
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
    public partial class FrmAddUnit : EVForm
    {
        public Unit Unit;
        public FrmAddUnit()
        {
            InitializeComponent();
        }

        private void FrmAddUnit_Load(object sender, EventArgs e)
        {
            panel2.Visible = false;
            if (this.Tag != null) tbName.Text = Tag.ToString();
        }

        private void BtnCreate_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(tbName.Text))
            {
                MessageBox.Show("请输入单元名称!");
                return;
            }

            if (!panel2.Visible)
            {
                Unit = new Unit(tbName.Text);
            }
            else
            {
                int angle = 0;
                if (rb0.Checked) angle = 0;
                else if (rb1.Checked) angle = 90;
                else if (rb2.Checked) angle = 180;
                else if (rb3.Checked) angle = 270;
                Unit = new UIUnit(tbName.Text, (int)nudLight.Value, angle, new Size((int)nudWidth.Value, (int)nudHight.Value));
            }
            DialogResult = DialogResult.OK;
            this.Close();
        }

        private void BtnCancel_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            this.Close();
        }

        private void IvProject_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (ivProject.SelectedItems.Count < 1)
                return;
            if (ivProject.SelectedItems[0].Index == 0)
            {
                panel2.Visible = false;
                tbName.Text = this.Tag.ToString();
                tbName.ReadOnly = false;
            }
            else if(ivProject.SelectedItems[0].Index == 1)
            {
                panel2.Visible = true;
                tbName.Text = "DisplayUnit";
                tbName.ReadOnly = true;
            }

        }

        private void Rb1_CheckedChanged(object sender, EventArgs e)
        {
            decimal tmp = nudWidth.Value;
            nudWidth.Value = nudHight.Value;
            nudHight.Value = tmp;
        }
    }
}
