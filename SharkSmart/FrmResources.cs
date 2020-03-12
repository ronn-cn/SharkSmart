using EVClassLib;
using EVTechnology.Common.Helper;
using System;
using System.Drawing;
using System.IO;
using System.Windows.Forms;

namespace SharkSmart
{
    public partial class FrmResources : Form
    {
        public ExecModule Mod { set; get; }

        private string FoldPath { set; get; } = "";
        public FrmResources()
        {
            InitializeComponent();
        }

        private void FrmResources_Load(object sender, EventArgs e)
        {
            FoldPath = Path.Combine(Engine.Project.WorkPath, Mod.Name, "Resources");
            ShowRes();
        }

        private void UpdateListView()
        {
            lvData.View = View.Details;
            lvData.Columns.Add("文件名称");
            lvData.Columns[0].Width = 200;
            lvData.Items.Clear();
            DirectoryInfo root = new DirectoryInfo(FoldPath);
            FileInfo[] files = root.GetFiles();
            foreach (var file in files)
            {
                ListViewItem item = new ListViewItem(file.Name);
                item.SubItems.Add(file.Name);
                //item.SubItems.Add(file.Name);
                //item.Text = file.Name;
                lvData.Items.Add(item);
            }
        }

        private void ShowRes()
        {
            lvData.Items.Clear();
            this.imageList1.Images.Clear();
            imageList1.Images.Add(Properties.Resources.mp3);
            imageList1.Images.Add(Properties.Resources.data);
            this.lvData.View = View.LargeIcon;
            this.lvData.LargeImageList = this.imageList1;
            this.lvData.BeginUpdate();

            string[] imageExt = new string[] { ".tif", ".png", ".jpg", ".bmp" };
            string[] audioExt = new string[] { ".mp3", ".wav" };
            string[] strs = new string[] { ".tif", ".png", ".jpg", ".bmp", ".mp3", ".wav", ".wma" };

            int index = 2;

            foreach (var res in Mod.Resources)
            {
                ListViewItem lvi = new ListViewItem();
                lvi.Text = res.Name;

                if (res.Type == ResType.Audio)
                {
                    lvi.ImageIndex = 0;
                }
                if (res.Type == ResType.File)
                {
                    lvi.ImageIndex = 1;
                }
                else if (res.Type == ResType.Image)
                {
                    Image image = FileHelper.GetImage(Path.GetDirectoryName(Mod.WorkPath) + "\\" + res.Path);
                    imageList1.Images.Add(image);//添加图片
                    lvi.ImageIndex = index++;
                }
                lvData.Items.Add(lvi);
            }
            this.lvData.EndUpdate();
        }

        private void TsbRefresh_Click(object sender, EventArgs e)
        {
            ShowRes();
        }

        private void TsbAdd_Click(object sender, EventArgs e)
        {
            FrmAddResources frm = new FrmAddResources();
            var a = frm.ShowDialog();
            if (a == DialogResult.OK)
            {
                Mod.AddRes(frm.Res);
                ShowRes();
            }
        }

        private void TsbDelete_Click(object sender, EventArgs e)
        {
            //删除资源
            if (lvData.SelectedItems.Count < 1) return;

            Resource res = Mod.Resources.Find(p => p.Name == Path.GetFileNameWithoutExtension(lvData.SelectedItems[0].Text));
            if (res != null)
            {
                Mod.DeleteRes(res);
                ShowRes();
            }
        }
    }
}
