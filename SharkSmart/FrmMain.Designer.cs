namespace SharkSmart
{
    partial class FrmMain
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
            EVTechnology.Common.Controls.ItemColors itemColors1 = new EVTechnology.Common.Controls.ItemColors();
            EVTechnology.Common.Controls.EVTabPage evTabPage1 = new EVTechnology.Common.Controls.EVTabPage();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FrmMain));
            this.tvExplorer = new System.Windows.Forms.TreeView();
            this.imageList = new System.Windows.Forms.ImageList(this.components);
            this.cmsTree = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.新建ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.tsmAddPlan = new System.Windows.Forms.ToolStripMenuItem();
            this.tsmAddUnit = new System.Windows.Forms.ToolStripMenuItem();
            this.tsmDelete = new System.Windows.Forms.ToolStripMenuItem();
            this.tsmDeleteMod = new System.Windows.Forms.ToolStripMenuItem();
            this.tsmOpen = new System.Windows.Forms.ToolStripMenuItem();
            this.tsmRename = new System.Windows.Forms.ToolStripMenuItem();
            this.tsmAddView = new System.Windows.Forms.ToolStripMenuItem();
            this.panel1 = new System.Windows.Forms.Panel();
            this.toolStripNoFrame1 = new EVTechnology.Common.Controls.ToolStripNoFrame();
            this.toolStripLabel1 = new System.Windows.Forms.ToolStripLabel();
            this.tsbRefresh = new System.Windows.Forms.ToolStripButton();
            this.panel2 = new System.Windows.Forms.Panel();
            this.tbMain = new EVTechnology.Common.Controls.EVTabControl();
            this.btnDebug = new System.Windows.Forms.Button();
            this.btnCatalog = new System.Windows.Forms.Button();
            this.btnAddPlan = new System.Windows.Forms.Button();
            this.btnAddUnit = new System.Windows.Forms.Button();
            this.btnSave = new System.Windows.Forms.Button();
            this.btnProject = new System.Windows.Forms.Button();
            this.TitleBar.SuspendLayout();
            this.cmsTree.SuspendLayout();
            this.panel1.SuspendLayout();
            this.toolStripNoFrame1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // TitleBar
            // 
            this.TitleBar.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(42)))), ((int)(((byte)(46)))), ((int)(((byte)(77)))));
            this.TitleBar.Controls.Add(this.btnSave);
            this.TitleBar.Controls.Add(this.btnProject);
            this.TitleBar.Controls.Add(this.btnAddPlan);
            this.TitleBar.Controls.Add(this.btnAddUnit);
            this.TitleBar.Controls.Add(this.btnCatalog);
            this.TitleBar.Controls.Add(this.btnDebug);
            this.TitleBar.Location = new System.Drawing.Point(4, 4);
            this.TitleBar.Size = new System.Drawing.Size(1089, 80);
            // 
            // tvExplorer
            // 
            this.tvExplorer.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(42)))), ((int)(((byte)(46)))), ((int)(((byte)(77)))));
            this.tvExplorer.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tvExplorer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tvExplorer.Font = new System.Drawing.Font("Microsoft Sans Serif", 13F);
            this.tvExplorer.ForeColor = System.Drawing.Color.White;
            this.tvExplorer.ImageIndex = 0;
            this.tvExplorer.ImageList = this.imageList;
            this.tvExplorer.LineColor = System.Drawing.Color.White;
            this.tvExplorer.Location = new System.Drawing.Point(0, 50);
            this.tvExplorer.Name = "tvExplorer";
            this.tvExplorer.SelectedImageIndex = 0;
            this.tvExplorer.ShowNodeToolTips = true;
            this.tvExplorer.ShowPlusMinus = false;
            this.tvExplorer.ShowRootLines = false;
            this.tvExplorer.Size = new System.Drawing.Size(290, 556);
            this.tvExplorer.TabIndex = 0;
            this.tvExplorer.NodeMouseClick += new System.Windows.Forms.TreeNodeMouseClickEventHandler(this.TvExplorer_NodeMouseClick);
            this.tvExplorer.NodeMouseDoubleClick += new System.Windows.Forms.TreeNodeMouseClickEventHandler(this.TvExplorer_NodeMouseDoubleClick);
            // 
            // imageList
            // 
            this.imageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList.ImageStream")));
            this.imageList.TransparentColor = System.Drawing.Color.Transparent;
            this.imageList.Images.SetKeyName(0, "wenjianjia.png");
            this.imageList.Images.SetKeyName(1, "daima.png");
            this.imageList.Images.SetKeyName(2, "tupian.png");
            this.imageList.Images.SetKeyName(3, "table.png");
            this.imageList.Images.SetKeyName(4, "unit.png");
            // 
            // cmsTree
            // 
            this.cmsTree.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.新建ToolStripMenuItem,
            this.tsmDelete,
            this.tsmDeleteMod,
            this.tsmOpen,
            this.tsmRename,
            this.tsmAddView});
            this.cmsTree.Name = "cmsTree";
            this.cmsTree.Size = new System.Drawing.Size(125, 136);
            // 
            // 新建ToolStripMenuItem
            // 
            this.新建ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tsmAddPlan,
            this.tsmAddUnit});
            this.新建ToolStripMenuItem.Name = "新建ToolStripMenuItem";
            this.新建ToolStripMenuItem.Size = new System.Drawing.Size(124, 22);
            this.新建ToolStripMenuItem.Text = "新建";
            // 
            // tsmAddPlan
            // 
            this.tsmAddPlan.Name = "tsmAddPlan";
            this.tsmAddPlan.Size = new System.Drawing.Size(100, 22);
            this.tsmAddPlan.Text = "计划";
            this.tsmAddPlan.Click += new System.EventHandler(this.TsmAddPlan_Click);
            // 
            // tsmAddUnit
            // 
            this.tsmAddUnit.Name = "tsmAddUnit";
            this.tsmAddUnit.Size = new System.Drawing.Size(100, 22);
            this.tsmAddUnit.Text = "单元";
            this.tsmAddUnit.Click += new System.EventHandler(this.TsmAddUnit_Click);
            // 
            // tsmDelete
            // 
            this.tsmDelete.Name = "tsmDelete";
            this.tsmDelete.Size = new System.Drawing.Size(124, 22);
            this.tsmDelete.Text = "删除";
            this.tsmDelete.Click += new System.EventHandler(this.TsmDelete_Click);
            // 
            // tsmDeleteMod
            // 
            this.tsmDeleteMod.Name = "tsmDeleteMod";
            this.tsmDeleteMod.Size = new System.Drawing.Size(124, 22);
            this.tsmDeleteMod.Text = "删除模块";
            this.tsmDeleteMod.Click += new System.EventHandler(this.TsmDeleteMod_Click);
            // 
            // tsmOpen
            // 
            this.tsmOpen.Name = "tsmOpen";
            this.tsmOpen.Size = new System.Drawing.Size(124, 22);
            this.tsmOpen.Text = "打开";
            this.tsmOpen.Click += new System.EventHandler(this.TsmOpen_Click);
            // 
            // tsmRename
            // 
            this.tsmRename.Name = "tsmRename";
            this.tsmRename.Size = new System.Drawing.Size(124, 22);
            this.tsmRename.Text = "重命名";
            this.tsmRename.Visible = false;
            this.tsmRename.Click += new System.EventHandler(this.TsmRename_Click);
            // 
            // tsmAddView
            // 
            this.tsmAddView.Name = "tsmAddView";
            this.tsmAddView.Size = new System.Drawing.Size(124, 22);
            this.tsmAddView.Text = "添加视图";
            this.tsmAddView.Click += new System.EventHandler(this.TsmAddView_Click);
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.tvExplorer);
            this.panel1.Controls.Add(this.toolStripNoFrame1);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Left;
            this.panel1.Location = new System.Drawing.Point(4, 84);
            this.panel1.Name = "panel1";
            this.panel1.Padding = new System.Windows.Forms.Padding(0, 10, 10, 0);
            this.panel1.Size = new System.Drawing.Size(300, 606);
            this.panel1.TabIndex = 1;
            // 
            // toolStripNoFrame1
            // 
            this.toolStripNoFrame1.AutoSize = false;
            this.toolStripNoFrame1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(49)))), ((int)(((byte)(57)))), ((int)(((byte)(94)))));
            this.toolStripNoFrame1.Font = new System.Drawing.Font("Microsoft YaHei UI", 12F);
            this.toolStripNoFrame1.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.toolStripNoFrame1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripLabel1,
            this.tsbRefresh});
            this.toolStripNoFrame1.Location = new System.Drawing.Point(0, 10);
            this.toolStripNoFrame1.Name = "toolStripNoFrame1";
            this.toolStripNoFrame1.RenderMode = System.Windows.Forms.ToolStripRenderMode.System;
            this.toolStripNoFrame1.Size = new System.Drawing.Size(290, 40);
            this.toolStripNoFrame1.TabIndex = 0;
            this.toolStripNoFrame1.Text = "toolStripNoFrame1";
            // 
            // toolStripLabel1
            // 
            this.toolStripLabel1.Name = "toolStripLabel1";
            this.toolStripLabel1.Size = new System.Drawing.Size(90, 37);
            this.toolStripLabel1.Text = "资源管理器";
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
            // panel2
            // 
            this.panel2.Controls.Add(this.tbMain);
            this.panel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel2.Location = new System.Drawing.Point(304, 84);
            this.panel2.Name = "panel2";
            this.panel2.Padding = new System.Windows.Forms.Padding(0, 10, 0, 0);
            this.panel2.Size = new System.Drawing.Size(789, 606);
            this.panel2.TabIndex = 2;
            // 
            // tbMain
            // 
            this.tbMain.AllowDragForm = true;
            this.tbMain.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(42)))), ((int)(((byte)(46)))), ((int)(((byte)(77)))));
            this.tbMain.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tbMain.Font = new System.Drawing.Font("Microsoft YaHei UI", 12F);
            this.tbMain.ForeColor = System.Drawing.Color.White;
            itemColors1.ItemActiveColor = System.Drawing.Color.FromArgb(((int)(((byte)(36)))), ((int)(((byte)(164)))), ((int)(((byte)(197)))));
            itemColors1.ItemBackColor = System.Drawing.Color.FromArgb(((int)(((byte)(49)))), ((int)(((byte)(57)))), ((int)(((byte)(94)))));
            itemColors1.ItemHoverColor = System.Drawing.SystemColors.ControlLight;
            this.tbMain.ItemColors = itemColors1;
            this.tbMain.ItemMargin = 0;
            evTabPage1.IsSaved = true;
            evTabPage1.TabIcon = null;
            evTabPage1.TabId = 0;
            evTabPage1.TabPath = null;
            evTabPage1.TabText = "Home";
            this.tbMain.Items.Add(evTabPage1);
            this.tbMain.ItemSize = new System.Drawing.Size(150, 40);
            this.tbMain.Location = new System.Drawing.Point(0, 10);
            this.tbMain.Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
            this.tbMain.Name = "tbMain";
            this.tbMain.Padding = new System.Windows.Forms.Padding(0, 40, 0, 0);
            this.tbMain.SelectedIndex = -1;
            this.tbMain.SelectedItem = null;
            this.tbMain.ShowCloseButton = true;
            this.tbMain.ShowSavedFlag = true;
            this.tbMain.Size = new System.Drawing.Size(789, 596);
            this.tbMain.TabIndex = 0;
            // 
            // btnDebug
            // 
            this.btnDebug.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnDebug.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(42)))), ((int)(((byte)(46)))), ((int)(((byte)(77)))));
            this.btnDebug.BackgroundImage = global::SharkSmart.Properties.Resources.bug;
            this.btnDebug.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.btnDebug.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.btnDebug.FlatAppearance.BorderSize = 0;
            this.btnDebug.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnDebug.Font = new System.Drawing.Font("宋体", 10F);
            this.btnDebug.ForeColor = System.Drawing.SystemColors.ButtonFace;
            this.btnDebug.ImageAlign = System.Drawing.ContentAlignment.TopCenter;
            this.btnDebug.Location = new System.Drawing.Point(990, 29);
            this.btnDebug.Name = "btnDebug";
            this.btnDebug.Size = new System.Drawing.Size(48, 48);
            this.btnDebug.TabIndex = 4;
            this.btnDebug.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.btnDebug.UseVisualStyleBackColor = false;
            this.btnDebug.Click += new System.EventHandler(this.BtnDebug_Click);
            // 
            // btnCatalog
            // 
            this.btnCatalog.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnCatalog.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(42)))), ((int)(((byte)(46)))), ((int)(((byte)(77)))));
            this.btnCatalog.BackgroundImage = global::SharkSmart.Properties.Resources.zu;
            this.btnCatalog.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.btnCatalog.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.btnCatalog.FlatAppearance.BorderSize = 0;
            this.btnCatalog.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnCatalog.Font = new System.Drawing.Font("宋体", 10F);
            this.btnCatalog.ForeColor = System.Drawing.SystemColors.ButtonFace;
            this.btnCatalog.ImageAlign = System.Drawing.ContentAlignment.TopCenter;
            this.btnCatalog.Location = new System.Drawing.Point(1038, 29);
            this.btnCatalog.Name = "btnCatalog";
            this.btnCatalog.Size = new System.Drawing.Size(48, 48);
            this.btnCatalog.TabIndex = 5;
            this.btnCatalog.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.btnCatalog.UseVisualStyleBackColor = false;
            this.btnCatalog.Click += new System.EventHandler(this.BtnCatalog_Click);
            // 
            // btnAddPlan
            // 
            this.btnAddPlan.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(42)))), ((int)(((byte)(46)))), ((int)(((byte)(77)))));
            this.btnAddPlan.BackgroundImage = global::SharkSmart.Properties.Resources.plan;
            this.btnAddPlan.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.btnAddPlan.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.btnAddPlan.FlatAppearance.BorderSize = 0;
            this.btnAddPlan.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnAddPlan.Font = new System.Drawing.Font("宋体", 10F);
            this.btnAddPlan.ForeColor = System.Drawing.SystemColors.ButtonFace;
            this.btnAddPlan.ImageAlign = System.Drawing.ContentAlignment.TopCenter;
            this.btnAddPlan.Location = new System.Drawing.Point(180, 29);
            this.btnAddPlan.Name = "btnAddPlan";
            this.btnAddPlan.Size = new System.Drawing.Size(48, 48);
            this.btnAddPlan.TabIndex = 2;
            this.btnAddPlan.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.btnAddPlan.UseVisualStyleBackColor = false;
            this.btnAddPlan.Visible = false;
            // 
            // btnAddUnit
            // 
            this.btnAddUnit.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(42)))), ((int)(((byte)(46)))), ((int)(((byte)(77)))));
            this.btnAddUnit.BackgroundImage = global::SharkSmart.Properties.Resources.unit;
            this.btnAddUnit.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.btnAddUnit.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.btnAddUnit.FlatAppearance.BorderSize = 0;
            this.btnAddUnit.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnAddUnit.Font = new System.Drawing.Font("宋体", 10F);
            this.btnAddUnit.ForeColor = System.Drawing.SystemColors.ButtonFace;
            this.btnAddUnit.ImageAlign = System.Drawing.ContentAlignment.TopCenter;
            this.btnAddUnit.Location = new System.Drawing.Point(228, 29);
            this.btnAddUnit.Name = "btnAddUnit";
            this.btnAddUnit.Size = new System.Drawing.Size(48, 48);
            this.btnAddUnit.TabIndex = 1;
            this.btnAddUnit.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.btnAddUnit.UseVisualStyleBackColor = false;
            this.btnAddUnit.Visible = false;
            // 
            // btnSave
            // 
            this.btnSave.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(42)))), ((int)(((byte)(46)))), ((int)(((byte)(77)))));
            this.btnSave.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("btnSave.BackgroundImage")));
            this.btnSave.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.btnSave.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.btnSave.FlatAppearance.BorderSize = 0;
            this.btnSave.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnSave.Font = new System.Drawing.Font("宋体", 10F);
            this.btnSave.ForeColor = System.Drawing.SystemColors.ButtonFace;
            this.btnSave.ImageAlign = System.Drawing.ContentAlignment.TopCenter;
            this.btnSave.Location = new System.Drawing.Point(132, 29);
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(48, 48);
            this.btnSave.TabIndex = 3;
            this.btnSave.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.btnSave.UseVisualStyleBackColor = false;
            this.btnSave.Click += new System.EventHandler(this.BtnSave_Click);
            // 
            // btnProject
            // 
            this.btnProject.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(42)))), ((int)(((byte)(46)))), ((int)(((byte)(77)))));
            this.btnProject.BackgroundImage = global::SharkSmart.Properties.Resources._1;
            this.btnProject.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.btnProject.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.btnProject.FlatAppearance.BorderSize = 0;
            this.btnProject.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnProject.Font = new System.Drawing.Font("宋体", 10F);
            this.btnProject.ForeColor = System.Drawing.SystemColors.ButtonFace;
            this.btnProject.ImageAlign = System.Drawing.ContentAlignment.TopCenter;
            this.btnProject.Location = new System.Drawing.Point(84, 29);
            this.btnProject.Name = "btnProject";
            this.btnProject.Size = new System.Drawing.Size(48, 48);
            this.btnProject.TabIndex = 0;
            this.btnProject.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.btnProject.UseVisualStyleBackColor = false;
            this.btnProject.Click += new System.EventHandler(this.BtnProject_Click);
            // 
            // FrmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(20)))), ((int)(((byte)(23)))), ((int)(((byte)(51)))));
            this.CaptionHeight = 80;
            this.ClientSize = new System.Drawing.Size(1097, 694);
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.panel1);
            this.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.ForeColor = System.Drawing.Color.White;
            this.Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
            this.Name = "FrmMain";
            this.Padding = new System.Windows.Forms.Padding(4);
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "FrmMain";
            this.TitleAlign = System.Drawing.ContentAlignment.TopCenter;
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.FrmMain_FormClosed);
            this.Load += new System.EventHandler(this.FrmMain_Load);
            this.Controls.SetChildIndex(this.TitleBar, 0);
            this.Controls.SetChildIndex(this.panel1, 0);
            this.Controls.SetChildIndex(this.panel2, 0);
            this.TitleBar.ResumeLayout(false);
            this.cmsTree.ResumeLayout(false);
            this.panel1.ResumeLayout(false);
            this.toolStripNoFrame1.ResumeLayout(false);
            this.toolStripNoFrame1.PerformLayout();
            this.panel2.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.TreeView tvExplorer;
        private System.Windows.Forms.Panel panel2;
        private EVTechnology.Common.Controls.EVTabControl tbMain;
        private System.Windows.Forms.ImageList imageList;
        private System.Windows.Forms.Button btnDebug;
        private System.Windows.Forms.Button btnCatalog;
        private System.Windows.Forms.Button btnSave;
        private System.Windows.Forms.Button btnProject;
        private System.Windows.Forms.Button btnAddPlan;
        private System.Windows.Forms.Button btnAddUnit;
        private EVTechnology.Common.Controls.ToolStripNoFrame toolStripNoFrame1;
        private System.Windows.Forms.ToolStripLabel toolStripLabel1;
        private System.Windows.Forms.ToolStripButton tsbRefresh;
        private System.Windows.Forms.ContextMenuStrip cmsTree;
        private System.Windows.Forms.ToolStripMenuItem 新建ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem tsmAddPlan;
        private System.Windows.Forms.ToolStripMenuItem tsmAddUnit;
        private System.Windows.Forms.ToolStripMenuItem tsmDelete;
        private System.Windows.Forms.ToolStripMenuItem tsmOpen;
        private System.Windows.Forms.ToolStripMenuItem tsmRename;
        private System.Windows.Forms.ToolStripMenuItem tsmAddView;
        private System.Windows.Forms.ToolStripMenuItem tsmDeleteMod;
    }
}