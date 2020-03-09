using EVClassLib;
using EVTechnology.Common.Controls;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
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
        
        private BaseEvent _event;

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
            deg.BlockClickedEvent += Deg_BlockClickedEvent;
            deg.BlockCreatedEvent += Deg_BlockCreatedEvent;
            Controls.Add(deg);
            deg.plExpand.Controls.Add(dgvEvent);
            dgvEvent.Dock = DockStyle.Fill;
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

        private SControl ctl;

        private void Deg_BlockClickedEvent()
        {
            if (deg.CurCvs.Selected != null)
            {
                //对应关系
                UIUnit unit = (UIUnit)((ExecModule)Mod).Units.Find(p => p.GetType() == typeof(UIUnit));
                ViewPage page = unit.Pages.Find(p => p.Name == Path.GetFileNameWithoutExtension(FilePath));
                ctl = page.Controls.Find(p => p.Name == page.Name + "_" + deg.CurCvs.Selected.Name);
                _event = ctl?.Event;
            }
            else
                _event = null;
            UpdateDataGridView();
        }
       
        public void UpdateDataGridView()
        {
            dgvEvent.Rows.Clear();
            if (dgvEvent == null) return;
            if (_event == null) return;
            foreach (PropertyInfo p in _event.GetType().GetProperties())
            {
                int index = dgvEvent.Rows.Add();
                dgvEvent.Rows[index].Cells[0].Value = ((DisplayNameAttribute)p.GetCustomAttribute(typeof(DisplayNameAttribute), false)).DisplayName;
                dgvEvent.Rows[index].Cells[1].Value = p.GetValue(_event);
                dgvEvent.Rows[index].Cells[1].Tag = p.Name;
            }
        }

        private void DgvEvent_CellValueChanged(object sender, DataGridViewCellEventArgs e)
        {
            if (_event == null || dgvEvent.SelectedCells.Count < 1 || dgvEvent.SelectedCells[0].Tag == null) return;
            Type tp = _event.GetType();
            tp.GetProperty(dgvEvent.SelectedCells[0].Tag.ToString()).SetValue(_event, dgvEvent.SelectedCells[0].Value);
            ((UIUnit)((ExecModule)Mod).Units.Find(p => p.GetType() == typeof(UIUnit))).
                Pages.Find(p => p.Name == Path.GetFileNameWithoutExtension(FilePath)).SetEvent(_event, deg.CurCvs.Selected.Name);
            Mod.Save();
        }

        private void DgvEvent_CellDoubleClick(object sender, DataGridViewCellEventArgs e)
        {
            if (dgvEvent.SelectedCells[0].ColumnIndex != 1)
                return;
            Unit unit = ((ExecModule)Mod).Units.Find(p => p.GetType() == typeof(UIUnit));
            if (unit == null) return;
            string path = Path.GetDirectoryName(Mod.WorkPath) + "\\Units\\" + unit.Name + ".py";
            string code = File.ReadAllText(path);

            DataGridViewCell cell = dgvEvent.SelectedCells[0];
            if (cell.Value == null)
            {
                string stra = Path.GetFileNameWithoutExtension(FilePath);
                cell.Value = stra + "_" + deg.CurCvs.Selected.Name + "_" + cell.Tag.ToString();
                string rowA = stra + "." + deg.CurCvs.Selected.Name 
                    + "." + cell.Tag.ToString() + " += " + cell.Value+ "\n";

                if (!code.Contains(rowA))
                {
                    string rowb = "def " + cell.Value + "():\n    pass";
                    code += "\n" + rowA + rowb;
                    File.WriteAllText(path, code);
                }
            }
            Mod.Save();
            Main.SkipCodeView(path);
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
