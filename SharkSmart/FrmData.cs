using EVClassLib;
using System;
using System.Windows.Forms;

namespace SharkSmart
{
    public partial class FrmData : Form
    {
        public ExecModule mod { set; get; }

        public FrmData()
        {
            InitializeComponent();
        }

        private void FrmAddData_Load(object sender, EventArgs e)
        {
            UpdateDataGridView();
        }


        private void UpdateDataGridView()
        {
            if (mod.Data == null) return;
            foreach (var data in mod.Data.Datas)
            {
                int index = dgvData.Rows.Add();
                dgvData.Rows[index].Cells[0].Value = data.Key;
                dgvData.Rows[index].Cells[1].Value = data.Value;
                //dgvData.Rows[index].Cells[2].Value = "监听";
            }
        }

        private void DgvData_CellValueChanged(object sender, DataGridViewCellEventArgs e)
        {

        }

        protected override bool ProcessCmdKey(ref Message msg, Keys keyData)
        {
            if (keyData == (Keys.S | Keys.Control))
            {
                if (mod.Data == null)
                    mod.Data = new DataBase();
                mod.Data.Datas.Clear();
                for (int i = 0; i < dgvData.RowCount; i++)
                {
                    if (dgvData.Rows[i].Cells[0].Value == null) continue;
                    else if (dgvData.Rows[i].Cells[1].Value == null) continue;
                    Data data = new Data(dgvData.Rows[i].Cells[0]?.Value.ToString(), dgvData.Rows[i].Cells[1]?.Value);
                    mod.Data.Datas.Add(data);
                }
                mod.Save();
            }
            return base.ProcessCmdKey(ref msg, keyData);
        }

        private void TsmDelete_Click(object sender, EventArgs e)
        {
            if (dgvData.SelectedCells[0] == null)
                return;
            for (int i = dgvData.SelectedRows.Count - 1; i >= 0; i--)
            {
                if (dgvData.SelectedRows[i].IsNewRow) continue;
                dgvData.Rows.Remove(dgvData.SelectedRows[i]);
            }
        }

        private void DgvData_CellMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                if (e.RowIndex >= 0)
                {
                    if (dgvData.Rows[e.RowIndex].Selected == false)
                    {
                        dgvData.ClearSelection();
                        dgvData.Rows[e.RowIndex].Selected = true;
                    }
                    contextMenuStrip.Show(MousePosition.X, MousePosition.Y);
                }
            }
        }

        private void FrmData_SizeChanged(object sender, EventArgs e)
        {
            int avgWidth = dgvData.Width / dgvData.Columns.Count;//求出每一列的header宽度
            for (int i = 0; i < this.dgvData.Columns.Count; i++)
            {
                this.dgvData.Columns[i].Width = avgWidth - 30;//设置每一列的宽度
            }
        }
    }
}
