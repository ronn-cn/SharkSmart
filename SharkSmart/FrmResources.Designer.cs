namespace SharkSmart
{
    partial class FrmResources
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FrmResources));
            this.lvData = new System.Windows.Forms.ListView();
            this.imageList1 = new System.Windows.Forms.ImageList(this.components);
            this.toolStripNoFrame1 = new EVTechnology.Common.Controls.ToolStripNoFrame();
            this.tsbRefresh = new System.Windows.Forms.ToolStripButton();
            this.toolStripDropDownButton1 = new System.Windows.Forms.ToolStripDropDownButton();
            this.图片ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.tsbAdd = new System.Windows.Forms.ToolStripButton();
            this.tsbDelete = new System.Windows.Forms.ToolStripButton();
            this.toolStripNoFrame1.SuspendLayout();
            this.SuspendLayout();
            // 
            // lvData
            // 
            this.lvData.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(42)))), ((int)(((byte)(46)))), ((int)(((byte)(77)))));
            this.lvData.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.lvData.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lvData.ForeColor = System.Drawing.Color.White;
            this.lvData.HideSelection = false;
            this.lvData.Location = new System.Drawing.Point(0, 40);
            this.lvData.Name = "lvData";
            this.lvData.Size = new System.Drawing.Size(1005, 614);
            this.lvData.TabIndex = 0;
            this.lvData.UseCompatibleStateImageBehavior = false;
            // 
            // imageList1
            // 
            this.imageList1.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList1.ImageStream")));
            this.imageList1.TransparentColor = System.Drawing.Color.Transparent;
            this.imageList1.Images.SetKeyName(0, "音频.png");
            this.imageList1.Images.SetKeyName(1, "data.png");
            // 
            // toolStripNoFrame1
            // 
            this.toolStripNoFrame1.AutoSize = false;
            this.toolStripNoFrame1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(49)))), ((int)(((byte)(57)))), ((int)(((byte)(94)))));
            this.toolStripNoFrame1.Font = new System.Drawing.Font("Microsoft YaHei UI", 11F);
            this.toolStripNoFrame1.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.toolStripNoFrame1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tsbRefresh,
            this.toolStripDropDownButton1,
            this.tsbAdd,
            this.tsbDelete});
            this.toolStripNoFrame1.Location = new System.Drawing.Point(0, 0);
            this.toolStripNoFrame1.Name = "toolStripNoFrame1";
            this.toolStripNoFrame1.RenderMode = System.Windows.Forms.ToolStripRenderMode.System;
            this.toolStripNoFrame1.Size = new System.Drawing.Size(1005, 40);
            this.toolStripNoFrame1.TabIndex = 1;
            this.toolStripNoFrame1.Text = "toolStripNoFrame1";
            // 
            // tsbRefresh
            // 
            this.tsbRefresh.Alignment = System.Windows.Forms.ToolStripItemAlignment.Right;
            this.tsbRefresh.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tsbRefresh.Image = global::SharkSmart.Properties.Resources.cached;
            this.tsbRefresh.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tsbRefresh.Name = "tsbRefresh";
            this.tsbRefresh.Size = new System.Drawing.Size(23, 37);
            this.tsbRefresh.Text = "刷新";
            this.tsbRefresh.Click += new System.EventHandler(this.TsbRefresh_Click);
            // 
            // toolStripDropDownButton1
            // 
            this.toolStripDropDownButton1.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.图片ToolStripMenuItem});
            this.toolStripDropDownButton1.ForeColor = System.Drawing.Color.White;
            this.toolStripDropDownButton1.Image = ((System.Drawing.Image)(resources.GetObject("toolStripDropDownButton1.Image")));
            this.toolStripDropDownButton1.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripDropDownButton1.Name = "toolStripDropDownButton1";
            this.toolStripDropDownButton1.Size = new System.Drawing.Size(68, 37);
            this.toolStripDropDownButton1.Text = "音频";
            this.toolStripDropDownButton1.Visible = false;
            // 
            // 图片ToolStripMenuItem
            // 
            this.图片ToolStripMenuItem.Name = "图片ToolStripMenuItem";
            this.图片ToolStripMenuItem.Size = new System.Drawing.Size(108, 24);
            this.图片ToolStripMenuItem.Text = "图片";
            // 
            // tsbAdd
            // 
            this.tsbAdd.ForeColor = System.Drawing.Color.White;
            this.tsbAdd.Image = global::SharkSmart.Properties.Resources.wenjiantianjia;
            this.tsbAdd.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tsbAdd.Name = "tsbAdd";
            this.tsbAdd.Size = new System.Drawing.Size(89, 37);
            this.tsbAdd.Text = "添加资源";
            this.tsbAdd.Click += new System.EventHandler(this.TsbAdd_Click);
            // 
            // tsbDelete
            // 
            this.tsbDelete.ForeColor = System.Drawing.Color.White;
            this.tsbDelete.Image = global::SharkSmart.Properties.Resources.delete;
            this.tsbDelete.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tsbDelete.Name = "tsbDelete";
            this.tsbDelete.Size = new System.Drawing.Size(89, 37);
            this.tsbDelete.Text = "删除资源";
            this.tsbDelete.Click += new System.EventHandler(this.TsbDelete_Click);
            // 
            // FrmResources
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(42)))), ((int)(((byte)(46)))), ((int)(((byte)(77)))));
            this.ClientSize = new System.Drawing.Size(1005, 654);
            this.Controls.Add(this.lvData);
            this.Controls.Add(this.toolStripNoFrame1);
            this.Font = new System.Drawing.Font("思源黑体 CN Normal", 11.25F);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "FrmResources";
            this.Text = "FrmResources";
            this.Load += new System.EventHandler(this.FrmResources_Load);
            this.toolStripNoFrame1.ResumeLayout(false);
            this.toolStripNoFrame1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListView lvData;
        private System.Windows.Forms.ImageList imageList1;
        private EVTechnology.Common.Controls.ToolStripNoFrame toolStripNoFrame1;
        private System.Windows.Forms.ToolStripButton tsbRefresh;
        private System.Windows.Forms.ToolStripButton tsbAdd;
        private System.Windows.Forms.ToolStripButton tsbDelete;
        private System.Windows.Forms.ToolStripDropDownButton toolStripDropDownButton1;
        private System.Windows.Forms.ToolStripMenuItem 图片ToolStripMenuItem;
    }
}