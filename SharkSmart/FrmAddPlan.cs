using EVClassLib;
using EVTechnology.Common.Controls;
using System;
using System.Windows.Forms;

namespace SharkSmart
{
    public partial class FrmAddPlan : EVForm
    {
        public Plan Plan;

        public ExecModule Mod;

        public FrmAddPlan()
        {
            InitializeComponent();
        }

        private void FrmAddUnit_Load(object sender, EventArgs e)
        {
            if (this.Tag != null) tbName.Text = Tag.ToString();
            if (Plan != null)
            {
                label3.Text = "修改计划";
                btnCreate.Text = "保存";
                tbName.Text = Plan.Name;
            }
            else
            {
                for (int i = 0; i < Mod.Plans.Count + 2; i++)
                {
                    Plan plan = Mod.Plans.Find(p => p.Name == "Plan" + i.ToString());
                    if (plan == null)
                    {
                        tbName.Text = "Plan" + i.ToString();
                    }
                }
            }
        }

        private void BtnCreate_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(tbName.Text))
            {
                MessageBox.Show("计划名称不为空！"); return;
            }
            //若是中文名称或者其他非法字符，则不允许通过
            if (Plan == null && Mod.GetNameCount(tbName.Text) > 0)
            {
                MessageBox.Show("计划名称已存在！"); return;
            }

            if (Plan == null)
                Plan = new Plan();

            Plan.Priority = GetNumber(0, 20);
            //Plan.Size = Convert.ToInt32(cbMemory.Text);
            //Plan.TimeSpan = timeSpan;
            //Plan.TimeStart = timeStart;
            //Plan.TimeEnd = timeEnd;
            Plan.PlanType = (PlanType)cbbType.SelectedIndex;
            Plan.Enabled = (EnabledType)cbbEnable.SelectedIndex;

            if (label3.Text.Equals("修改计划"))
                Mod.ModuleFileRename(Plan, tbName.Text);
            else
            {
                Plan.Name = tbName.Text;
            }
            DialogResult = DialogResult.OK;
            this.Close();
        }

        private int GetNumber(int min, int max)
        {
            int num = min;
            int count = Mod.Plans.Count;
            for (int i = 0; i < count; i++)
            {
                if (num == Convert.ToInt32(Mod.Plans[i].Priority))
                    num++;
            }
            if (num >= max && num <= 90)
                GetNumber(max + 1, max + 7);
            else if (num > 90)
                return -1;
            return num;
        }

        private void BtnCancel_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            this.Close();
        }
    }
}
