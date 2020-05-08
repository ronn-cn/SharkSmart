namespace SharkSmart
{
    partial class FrmStartPage
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
			EVTechnology.Controls.ItemColors itemColors7 = new EVTechnology.Controls.ItemColors();
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FrmStartPage));
			EVTechnology.Controls.ItemColors itemColors8 = new EVTechnology.Controls.ItemColors();
			this.label10 = new System.Windows.Forms.Label();
			this.btnBrowse = new System.Windows.Forms.Button();
			this.ivProject = new EVTechnology.Controls.EVIconView();
			this.imgProject = new System.Windows.Forms.ImageList(this.components);
			this.plA = new System.Windows.Forms.Panel();
			this.btnOpen = new System.Windows.Forms.Button();
			this.llQuickCreate = new System.Windows.Forms.LinkLabel();
			this.label2 = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.tbProName = new System.Windows.Forms.TextBox();
			this.tbPath = new System.Windows.Forms.TextBox();
			this.btnNextOne = new System.Windows.Forms.Button();
			this.plB = new System.Windows.Forms.Panel();
			this.button1 = new System.Windows.Forms.Button();
			this.ivComponent = new System.Windows.Forms.ListView();
			this.btnDelete = new System.Windows.Forms.Button();
			this.btnAdd = new System.Windows.Forms.Button();
			this.label1 = new System.Windows.Forms.Label();
			this.btnNextTwo = new System.Windows.Forms.Button();
			this.cmbPreset = new System.Windows.Forms.ComboBox();
			this.label7 = new System.Windows.Forms.Label();
			this.btnPre = new System.Windows.Forms.Button();
			this.tbName = new System.Windows.Forms.TextBox();
			this.label6 = new System.Windows.Forms.Label();
			this.plC = new System.Windows.Forms.Panel();
			this.tvPreview = new EVTechnology.Controls.EVTreeView();
			this.label4 = new System.Windows.Forms.Label();
			this.btnCreate = new System.Windows.Forms.Button();
			this.btnPreTwo = new System.Windows.Forms.Button();
			this.panel4 = new System.Windows.Forms.Panel();
			this.label5 = new System.Windows.Forms.Label();
			this.cmsMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
			this.tsmRename = new System.Windows.Forms.ToolStripMenuItem();
			this.tsmDeleteRecord = new System.Windows.Forms.ToolStripMenuItem();
			this.tsmDeleteProject = new System.Windows.Forms.ToolStripMenuItem();
			this.label8 = new System.Windows.Forms.Label();
			this.label9 = new System.Windows.Forms.Label();
			this.label11 = new System.Windows.Forms.Label();
			this.plA.SuspendLayout();
			this.plB.SuspendLayout();
			this.plC.SuspendLayout();
			this.panel4.SuspendLayout();
			this.cmsMenu.SuspendLayout();
			this.SuspendLayout();
			// 
			// TitleBar
			// 
			this.TitleBar.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(41)))), ((int)(((byte)(49)))), ((int)(((byte)(82)))));
			this.TitleBar.Font = new System.Drawing.Font("Impact", 24.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.TitleBar.ForeColor = System.Drawing.Color.WhiteSmoke;
			this.TitleBar.Location = new System.Drawing.Point(4, 4);
			this.TitleBar.Size = new System.Drawing.Size(1192, 60);
			// 
			// label10
			// 
			this.label10.AutoSize = true;
			this.label10.Font = new System.Drawing.Font("Microsoft YaHei UI", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.label10.ForeColor = System.Drawing.Color.White;
			this.label10.Location = new System.Drawing.Point(75, 42);
			this.label10.Name = "label10";
			this.label10.Size = new System.Drawing.Size(145, 26);
			this.label10.TabIndex = 16;
			this.label10.Text = "最近打开的场景";
			// 
			// btnBrowse
			// 
			this.btnBrowse.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.btnBrowse.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(107)))), ((int)(((byte)(119)))), ((int)(((byte)(141)))));
			this.btnBrowse.FlatAppearance.BorderSize = 0;
			this.btnBrowse.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.btnBrowse.Font = new System.Drawing.Font("Microsoft YaHei UI", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.btnBrowse.ForeColor = System.Drawing.Color.White;
			this.btnBrowse.Location = new System.Drawing.Point(3, 684);
			this.btnBrowse.Name = "btnBrowse";
			this.btnBrowse.Size = new System.Drawing.Size(294, 36);
			this.btnBrowse.TabIndex = 8;
			this.btnBrowse.Text = "浏览本地";
			this.btnBrowse.UseVisualStyleBackColor = false;
			this.btnBrowse.Click += new System.EventHandler(this.BtnBrowse_Click);
			// 
			// ivProject
			// 
			this.ivProject.AutoArrange = false;
			this.ivProject.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(21)))), ((int)(((byte)(33)))), ((int)(((byte)(70)))));
			this.ivProject.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.ivProject.Cursor = System.Windows.Forms.Cursors.Default;
			this.ivProject.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F);
			this.ivProject.ForeColor = System.Drawing.Color.White;
			this.ivProject.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
			this.ivProject.HideSelection = false;
			itemColors7.ItemActiveColor = System.Drawing.Color.FromArgb(((int)(((byte)(231)))), ((int)(((byte)(234)))), ((int)(((byte)(246)))));
			itemColors7.ItemBackColor = System.Drawing.Color.FromArgb(((int)(((byte)(107)))), ((int)(((byte)(119)))), ((int)(((byte)(141)))));
			itemColors7.ItemHoverColor = System.Drawing.Color.FromArgb(((int)(((byte)(41)))), ((int)(((byte)(49)))), ((int)(((byte)(82)))));
			this.ivProject.ItemColors = itemColors7;
			this.ivProject.ItemRadius = 5;
			this.ivProject.ItemSize = new System.Drawing.Size(290, 50);
			this.ivProject.LabelWrap = false;
			this.ivProject.Location = new System.Drawing.Point(5, 106);
			this.ivProject.MultiSelect = false;
			this.ivProject.Name = "ivProject";
			this.ivProject.OwnerDraw = true;
			this.ivProject.ShowGroups = false;
			this.ivProject.ShowItemToolTips = true;
			this.ivProject.Size = new System.Drawing.Size(292, 564);
			this.ivProject.TabIndex = 17;
			this.ivProject.TileSize = new System.Drawing.Size(290, 50);
			this.ivProject.UseCompatibleStateImageBehavior = false;
			this.ivProject.View = System.Windows.Forms.View.Tile;
			this.ivProject.ItemCheck += new System.Windows.Forms.ItemCheckEventHandler(this.IvProject_ItemCheck);
			this.ivProject.MouseClick += new System.Windows.Forms.MouseEventHandler(this.IvProject_MouseClick);
			this.ivProject.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.IvProject_MouseDoubleClick);
			// 
			// imgProject
			// 
			this.imgProject.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imgProject.ImageStream")));
			this.imgProject.TransparentColor = System.Drawing.Color.Transparent;
			this.imgProject.Images.SetKeyName(0, "board-icon.png");
			// 
			// plA
			// 
			this.plA.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(32)))), ((int)(((byte)(45)))), ((int)(((byte)(89)))));
			this.plA.Controls.Add(this.btnOpen);
			this.plA.Controls.Add(this.llQuickCreate);
			this.plA.Controls.Add(this.label2);
			this.plA.Controls.Add(this.label3);
			this.plA.Controls.Add(this.tbProName);
			this.plA.Controls.Add(this.tbPath);
			this.plA.Controls.Add(this.btnNextOne);
			this.plA.Location = new System.Drawing.Point(329, 203);
			this.plA.Name = "plA";
			this.plA.Size = new System.Drawing.Size(250, 554);
			this.plA.TabIndex = 18;
			// 
			// btnOpen
			// 
			this.btnOpen.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(80)))), ((int)(((byte)(141)))));
			this.btnOpen.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.btnOpen.FlatAppearance.BorderSize = 0;
			this.btnOpen.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.btnOpen.Font = new System.Drawing.Font("Microsoft Sans Serif", 11F);
			this.btnOpen.ForeColor = System.Drawing.Color.Gainsboro;
			this.btnOpen.Location = new System.Drawing.Point(177, 127);
			this.btnOpen.Name = "btnOpen";
			this.btnOpen.Size = new System.Drawing.Size(60, 25);
			this.btnOpen.TabIndex = 17;
			this.btnOpen.Text = "▪▪▪";
			this.btnOpen.TextAlign = System.Drawing.ContentAlignment.TopCenter;
			this.btnOpen.UseVisualStyleBackColor = false;
			this.btnOpen.Click += new System.EventHandler(this.BtnOpen_Click);
			// 
			// llQuickCreate
			// 
			this.llQuickCreate.AutoSize = true;
			this.llQuickCreate.BackColor = System.Drawing.Color.Transparent;
			this.llQuickCreate.Font = new System.Drawing.Font("Microsoft YaHei UI", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.llQuickCreate.LinkColor = System.Drawing.Color.LightSkyBlue;
			this.llQuickCreate.Location = new System.Drawing.Point(57, 485);
			this.llQuickCreate.Name = "llQuickCreate";
			this.llQuickCreate.Size = new System.Drawing.Size(141, 20);
			this.llQuickCreate.TabIndex = 29;
			this.llQuickCreate.TabStop = true;
			this.llQuickCreate.Text = "空场景，快速创建>>";
			this.llQuickCreate.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.LlQuickCreate_LinkClicked);
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Font = new System.Drawing.Font("Microsoft YaHei UI", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.label2.ForeColor = System.Drawing.Color.White;
			this.label2.Location = new System.Drawing.Point(9, 61);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(68, 20);
			this.label2.TabIndex = 13;
			this.label2.Text = "场景名称:";
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Font = new System.Drawing.Font("Microsoft YaHei UI", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.label3.ForeColor = System.Drawing.Color.White;
			this.label3.Location = new System.Drawing.Point(9, 132);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(68, 20);
			this.label3.TabIndex = 15;
			this.label3.Text = "储存路径:";
			// 
			// tbProName
			// 
			this.tbProName.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(43)))), ((int)(((byte)(59)))), ((int)(((byte)(110)))));
			this.tbProName.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.tbProName.Font = new System.Drawing.Font("Microsoft Sans Serif", 11F);
			this.tbProName.ForeColor = System.Drawing.Color.White;
			this.tbProName.Location = new System.Drawing.Point(13, 85);
			this.tbProName.Name = "tbProName";
			this.tbProName.Size = new System.Drawing.Size(225, 24);
			this.tbProName.TabIndex = 14;
			this.tbProName.TextChanged += new System.EventHandler(this.TbProName_TextChanged);
			// 
			// tbPath
			// 
			this.tbPath.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(43)))), ((int)(((byte)(59)))), ((int)(((byte)(110)))));
			this.tbPath.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.tbPath.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F);
			this.tbPath.ForeColor = System.Drawing.Color.White;
			this.tbPath.Location = new System.Drawing.Point(13, 156);
			this.tbPath.Name = "tbPath";
			this.tbPath.Size = new System.Drawing.Size(225, 23);
			this.tbPath.TabIndex = 16;
			// 
			// btnNextOne
			// 
			this.btnNextOne.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(15)))), ((int)(((byte)(192)))), ((int)(((byte)(98)))));
			this.btnNextOne.FlatAppearance.BorderSize = 0;
			this.btnNextOne.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.btnNextOne.Font = new System.Drawing.Font("Microsoft YaHei UI", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.btnNextOne.ForeColor = System.Drawing.Color.White;
			this.btnNextOne.Location = new System.Drawing.Point(3, 508);
			this.btnNextOne.Name = "btnNextOne";
			this.btnNextOne.Size = new System.Drawing.Size(244, 36);
			this.btnNextOne.TabIndex = 18;
			this.btnNextOne.Text = "下一步";
			this.btnNextOne.UseVisualStyleBackColor = false;
			this.btnNextOne.Click += new System.EventHandler(this.BtnNextOne_Click);
			// 
			// plB
			// 
			this.plB.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(32)))), ((int)(((byte)(45)))), ((int)(((byte)(89)))));
			this.plB.Controls.Add(this.button1);
			this.plB.Controls.Add(this.ivComponent);
			this.plB.Controls.Add(this.btnDelete);
			this.plB.Controls.Add(this.btnAdd);
			this.plB.Controls.Add(this.label1);
			this.plB.Controls.Add(this.btnNextTwo);
			this.plB.Controls.Add(this.cmbPreset);
			this.plB.Controls.Add(this.label7);
			this.plB.Controls.Add(this.btnPre);
			this.plB.Controls.Add(this.tbName);
			this.plB.Controls.Add(this.label6);
			this.plB.Location = new System.Drawing.Point(629, 203);
			this.plB.Name = "plB";
			this.plB.Size = new System.Drawing.Size(250, 554);
			this.plB.TabIndex = 19;
			this.plB.EnabledChanged += new System.EventHandler(this.plB_EnabledChanged);
			// 
			// button1
			// 
			this.button1.Location = new System.Drawing.Point(211, 55);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(23, 23);
			this.button1.TabIndex = 31;
			this.button1.Text = "+";
			this.button1.UseVisualStyleBackColor = true;
			this.button1.Visible = false;
			this.button1.Click += new System.EventHandler(this.button1_Click);
			// 
			// ivComponent
			// 
			this.ivComponent.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(43)))), ((int)(((byte)(59)))), ((int)(((byte)(110)))));
			this.ivComponent.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.ivComponent.Font = new System.Drawing.Font("Microsoft YaHei UI", 11F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.ivComponent.ForeColor = System.Drawing.Color.White;
			this.ivComponent.HideSelection = false;
			this.ivComponent.Location = new System.Drawing.Point(9, 239);
			this.ivComponent.Name = "ivComponent";
			this.ivComponent.Size = new System.Drawing.Size(229, 251);
			this.ivComponent.SmallImageList = this.imgProject;
			this.ivComponent.TabIndex = 30;
			this.ivComponent.UseCompatibleStateImageBehavior = false;
			this.ivComponent.View = System.Windows.Forms.View.SmallIcon;
			// 
			// btnDelete
			// 
			this.btnDelete.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(230)))), ((int)(((byte)(70)))), ((int)(((byte)(58)))));
			this.btnDelete.FlatAppearance.BorderSize = 0;
			this.btnDelete.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.btnDelete.Font = new System.Drawing.Font("Microsoft YaHei UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.btnDelete.ForeColor = System.Drawing.Color.White;
			this.btnDelete.Location = new System.Drawing.Point(174, 198);
			this.btnDelete.Name = "btnDelete";
			this.btnDelete.Size = new System.Drawing.Size(60, 25);
			this.btnDelete.TabIndex = 29;
			this.btnDelete.Text = "删除";
			this.btnDelete.UseVisualStyleBackColor = false;
			this.btnDelete.Click += new System.EventHandler(this.BtnDelete_Click);
			// 
			// btnAdd
			// 
			this.btnAdd.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(21)))), ((int)(((byte)(79)))), ((int)(((byte)(255)))));
			this.btnAdd.FlatAppearance.BorderSize = 0;
			this.btnAdd.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.btnAdd.Font = new System.Drawing.Font("Microsoft YaHei UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.btnAdd.ForeColor = System.Drawing.Color.Gainsboro;
			this.btnAdd.Location = new System.Drawing.Point(108, 198);
			this.btnAdd.Name = "btnAdd";
			this.btnAdd.Size = new System.Drawing.Size(60, 25);
			this.btnAdd.TabIndex = 28;
			this.btnAdd.Text = "添加";
			this.btnAdd.UseVisualStyleBackColor = false;
			this.btnAdd.Click += new System.EventHandler(this.BtnAdd_Click);
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Font = new System.Drawing.Font("Microsoft YaHei UI", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.label1.ForeColor = System.Drawing.Color.White;
			this.label1.Location = new System.Drawing.Point(9, 198);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(68, 20);
			this.label1.TabIndex = 25;
			this.label1.Text = "包含模块:";
			// 
			// btnNextTwo
			// 
			this.btnNextTwo.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(15)))), ((int)(((byte)(192)))), ((int)(((byte)(98)))));
			this.btnNextTwo.FlatAppearance.BorderSize = 0;
			this.btnNextTwo.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.btnNextTwo.Font = new System.Drawing.Font("Microsoft YaHei UI", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.btnNextTwo.ForeColor = System.Drawing.Color.White;
			this.btnNextTwo.Location = new System.Drawing.Point(3, 508);
			this.btnNextTwo.Name = "btnNextTwo";
			this.btnNextTwo.Size = new System.Drawing.Size(244, 36);
			this.btnNextTwo.TabIndex = 26;
			this.btnNextTwo.Text = "下一步";
			this.btnNextTwo.UseVisualStyleBackColor = false;
			this.btnNextTwo.Click += new System.EventHandler(this.BtnNextTwo_Click);
			// 
			// cmbPreset
			// 
			this.cmbPreset.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(43)))), ((int)(((byte)(59)))), ((int)(((byte)(110)))));
			this.cmbPreset.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.cmbPreset.Font = new System.Drawing.Font("Microsoft Sans Serif", 11F);
			this.cmbPreset.ForeColor = System.Drawing.Color.White;
			this.cmbPreset.FormattingEnabled = true;
			this.cmbPreset.Location = new System.Drawing.Point(9, 84);
			this.cmbPreset.Name = "cmbPreset";
			this.cmbPreset.Size = new System.Drawing.Size(225, 26);
			this.cmbPreset.TabIndex = 23;
			this.cmbPreset.SelectedIndexChanged += new System.EventHandler(this.CmbPreset_SelectedIndexChanged);
			// 
			// label7
			// 
			this.label7.AutoSize = true;
			this.label7.Font = new System.Drawing.Font("Microsoft YaHei UI", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.label7.ForeColor = System.Drawing.Color.White;
			this.label7.Location = new System.Drawing.Point(9, 56);
			this.label7.Name = "label7";
			this.label7.Size = new System.Drawing.Size(68, 20);
			this.label7.TabIndex = 22;
			this.label7.Text = "选择预设:";
			// 
			// btnPre
			// 
			this.btnPre.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(80)))), ((int)(((byte)(141)))));
			this.btnPre.FlatAppearance.BorderSize = 0;
			this.btnPre.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.btnPre.Font = new System.Drawing.Font("Microsoft Sans Serif", 11F);
			this.btnPre.ForeColor = System.Drawing.Color.White;
			this.btnPre.Location = new System.Drawing.Point(3, 3);
			this.btnPre.Name = "btnPre";
			this.btnPre.Size = new System.Drawing.Size(30, 30);
			this.btnPre.TabIndex = 27;
			this.btnPre.Text = "◁";
			this.btnPre.UseVisualStyleBackColor = false;
			this.btnPre.Click += new System.EventHandler(this.BtnPre_Click);
			// 
			// tbName
			// 
			this.tbName.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(43)))), ((int)(((byte)(59)))), ((int)(((byte)(110)))));
			this.tbName.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.tbName.Font = new System.Drawing.Font("Microsoft Sans Serif", 11F);
			this.tbName.ForeColor = System.Drawing.Color.White;
			this.tbName.Location = new System.Drawing.Point(9, 156);
			this.tbName.Name = "tbName";
			this.tbName.Size = new System.Drawing.Size(225, 24);
			this.tbName.TabIndex = 21;
			// 
			// label6
			// 
			this.label6.AutoSize = true;
			this.label6.Font = new System.Drawing.Font("Microsoft YaHei UI", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.label6.ForeColor = System.Drawing.Color.White;
			this.label6.Location = new System.Drawing.Point(9, 132);
			this.label6.Name = "label6";
			this.label6.Size = new System.Drawing.Size(68, 20);
			this.label6.TabIndex = 20;
			this.label6.Text = "模块名称:";
			// 
			// plC
			// 
			this.plC.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(32)))), ((int)(((byte)(45)))), ((int)(((byte)(89)))));
			this.plC.Controls.Add(this.tvPreview);
			this.plC.Controls.Add(this.label4);
			this.plC.Controls.Add(this.btnCreate);
			this.plC.Controls.Add(this.btnPreTwo);
			this.plC.Location = new System.Drawing.Point(929, 203);
			this.plC.Name = "plC";
			this.plC.Size = new System.Drawing.Size(250, 554);
			this.plC.TabIndex = 20;
			// 
			// tvPreview
			// 
			this.tvPreview.AutoScroll = true;
			this.tvPreview.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(43)))), ((int)(((byte)(59)))), ((int)(((byte)(110)))));
			this.tvPreview.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F);
			itemColors8.ItemActiveColor = System.Drawing.Color.FromArgb(((int)(((byte)(231)))), ((int)(((byte)(234)))), ((int)(((byte)(246)))));
			itemColors8.ItemBackColor = System.Drawing.Color.FromArgb(((int)(((byte)(107)))), ((int)(((byte)(119)))), ((int)(((byte)(141)))));
			itemColors8.ItemHoverColor = System.Drawing.SystemColors.ControlLight;
			this.tvPreview.ItemColors = itemColors8;
			this.tvPreview.Location = new System.Drawing.Point(9, 85);
			this.tvPreview.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
			this.tvPreview.Name = "tvPreview";
			this.tvPreview.SelectedItem = null;
			this.tvPreview.Size = new System.Drawing.Size(231, 405);
			this.tvPreview.TabIndex = 29;
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Font = new System.Drawing.Font("Microsoft YaHei UI", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.label4.ForeColor = System.Drawing.Color.White;
			this.label4.Location = new System.Drawing.Point(14, 56);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(68, 20);
			this.label4.TabIndex = 28;
			this.label4.Text = "结构预览:";
			// 
			// btnCreate
			// 
			this.btnCreate.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(15)))), ((int)(((byte)(192)))), ((int)(((byte)(98)))));
			this.btnCreate.FlatAppearance.BorderSize = 0;
			this.btnCreate.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.btnCreate.Font = new System.Drawing.Font("Microsoft YaHei UI", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.btnCreate.ForeColor = System.Drawing.Color.White;
			this.btnCreate.Location = new System.Drawing.Point(3, 508);
			this.btnCreate.Name = "btnCreate";
			this.btnCreate.Size = new System.Drawing.Size(244, 36);
			this.btnCreate.TabIndex = 27;
			this.btnCreate.Text = "完成创建";
			this.btnCreate.UseVisualStyleBackColor = false;
			this.btnCreate.Click += new System.EventHandler(this.BtnCreate_Click);
			// 
			// btnPreTwo
			// 
			this.btnPreTwo.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(80)))), ((int)(((byte)(141)))));
			this.btnPreTwo.FlatAppearance.BorderSize = 0;
			this.btnPreTwo.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.btnPreTwo.Font = new System.Drawing.Font("Microsoft Sans Serif", 11F);
			this.btnPreTwo.ForeColor = System.Drawing.Color.White;
			this.btnPreTwo.Location = new System.Drawing.Point(3, 3);
			this.btnPreTwo.Name = "btnPreTwo";
			this.btnPreTwo.Size = new System.Drawing.Size(30, 30);
			this.btnPreTwo.TabIndex = 28;
			this.btnPreTwo.Text = "◁";
			this.btnPreTwo.UseVisualStyleBackColor = false;
			this.btnPreTwo.Click += new System.EventHandler(this.BtnPreTwo_Click);
			// 
			// panel4
			// 
			this.panel4.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(21)))), ((int)(((byte)(33)))), ((int)(((byte)(70)))));
			this.panel4.Controls.Add(this.label10);
			this.panel4.Controls.Add(this.btnBrowse);
			this.panel4.Controls.Add(this.ivProject);
			this.panel4.Dock = System.Windows.Forms.DockStyle.Left;
			this.panel4.Location = new System.Drawing.Point(4, 64);
			this.panel4.Name = "panel4";
			this.panel4.Size = new System.Drawing.Size(300, 732);
			this.panel4.TabIndex = 22;
			// 
			// label5
			// 
			this.label5.AutoSize = true;
			this.label5.Font = new System.Drawing.Font("Microsoft YaHei UI", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label5.ForeColor = System.Drawing.Color.White;
			this.label5.Location = new System.Drawing.Point(667, 106);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(164, 26);
			this.label5.TabIndex = 30;
			this.label5.Text = "创建一个新的场景";
			// 
			// cmsMenu
			// 
			this.cmsMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tsmRename,
            this.tsmDeleteRecord,
            this.tsmDeleteProject});
			this.cmsMenu.Name = "contextMenuStrip1";
			this.cmsMenu.Size = new System.Drawing.Size(125, 70);
			// 
			// tsmRename
			// 
			this.tsmRename.Name = "tsmRename";
			this.tsmRename.Size = new System.Drawing.Size(124, 22);
			this.tsmRename.Text = "重命名";
			this.tsmRename.Visible = false;
			// 
			// tsmDeleteRecord
			// 
			this.tsmDeleteRecord.Name = "tsmDeleteRecord";
			this.tsmDeleteRecord.Size = new System.Drawing.Size(124, 22);
			this.tsmDeleteRecord.Text = "删除记录";
			this.tsmDeleteRecord.Click += new System.EventHandler(this.TsmDeleteRecord_Click);
			// 
			// tsmDeleteProject
			// 
			this.tsmDeleteProject.Name = "tsmDeleteProject";
			this.tsmDeleteProject.Size = new System.Drawing.Size(124, 22);
			this.tsmDeleteProject.Text = "卸载项目";
			this.tsmDeleteProject.Click += new System.EventHandler(this.TsmDeleteProject_Click);
			// 
			// label8
			// 
			this.label8.AutoSize = true;
			this.label8.Font = new System.Drawing.Font("Microsoft YaHei UI", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.label8.ForeColor = System.Drawing.Color.Gainsboro;
			this.label8.Location = new System.Drawing.Point(395, 170);
			this.label8.Name = "label8";
			this.label8.Size = new System.Drawing.Size(99, 20);
			this.label8.TabIndex = 31;
			this.label8.Text = "一、创建场景";
			// 
			// label9
			// 
			this.label9.AutoSize = true;
			this.label9.Font = new System.Drawing.Font("Microsoft YaHei UI", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.label9.ForeColor = System.Drawing.Color.Gainsboro;
			this.label9.Location = new System.Drawing.Point(694, 170);
			this.label9.Name = "label9";
			this.label9.Size = new System.Drawing.Size(99, 20);
			this.label9.TabIndex = 32;
			this.label9.Text = "二、添加模块";
			// 
			// label11
			// 
			this.label11.AutoSize = true;
			this.label11.Font = new System.Drawing.Font("Microsoft YaHei UI", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.label11.ForeColor = System.Drawing.Color.Gainsboro;
			this.label11.Location = new System.Drawing.Point(993, 170);
			this.label11.Name = "label11";
			this.label11.Size = new System.Drawing.Size(99, 20);
			this.label11.TabIndex = 33;
			this.label11.Text = "三、预览创建";
			// 
			// FrmStartPage
			// 
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
			this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(5)))), ((int)(((byte)(18)))), ((int)(((byte)(49)))));
			this.CaptionHeight = 60;
			this.ClientSize = new System.Drawing.Size(1200, 800);
			this.Controls.Add(this.label11);
			this.Controls.Add(this.label9);
			this.Controls.Add(this.label8);
			this.Controls.Add(this.panel4);
			this.Controls.Add(this.label5);
			this.Controls.Add(this.plC);
			this.Controls.Add(this.plB);
			this.Controls.Add(this.plA);
			this.Font = new System.Drawing.Font("微软雅黑", 11F);
			this.ForeColor = System.Drawing.Color.White;
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MaximizeBox = false;
			this.MaximumSize = new System.Drawing.Size(1200, 800);
			this.Name = "FrmStartPage";
			this.Padding = new System.Windows.Forms.Padding(4);
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "SharkSmart";
			this.Load += new System.EventHandler(this.FrmStartStyle_Load);
			this.Controls.SetChildIndex(this.plA, 0);
			this.Controls.SetChildIndex(this.plB, 0);
			this.Controls.SetChildIndex(this.plC, 0);
			this.Controls.SetChildIndex(this.label5, 0);
			this.Controls.SetChildIndex(this.TitleBar, 0);
			this.Controls.SetChildIndex(this.panel4, 0);
			this.Controls.SetChildIndex(this.label8, 0);
			this.Controls.SetChildIndex(this.label9, 0);
			this.Controls.SetChildIndex(this.label11, 0);
			this.plA.ResumeLayout(false);
			this.plA.PerformLayout();
			this.plB.ResumeLayout(false);
			this.plB.PerformLayout();
			this.plC.ResumeLayout(false);
			this.plC.PerformLayout();
			this.panel4.ResumeLayout(false);
			this.panel4.PerformLayout();
			this.cmsMenu.ResumeLayout(false);
			this.ResumeLayout(false);
			this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Button btnBrowse;
        private EVTechnology.Controls.EVIconView ivProject;
        private System.Windows.Forms.Panel plA;
        private System.Windows.Forms.Panel plB;
        private System.Windows.Forms.Panel plC;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox tbProName;
        private System.Windows.Forms.TextBox tbPath;
        private System.Windows.Forms.Button btnOpen;
        private System.Windows.Forms.ComboBox cmbPreset;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox tbName;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btnNextTwo;
        private System.Windows.Forms.Button btnPre;
        private System.Windows.Forms.Button btnCreate;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Panel panel4;
        private System.Windows.Forms.Button btnPreTwo;
        private System.Windows.Forms.LinkLabel llQuickCreate;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Button btnAdd;
        private System.Windows.Forms.Button btnDelete;
        private System.Windows.Forms.ImageList imgProject;
        private System.Windows.Forms.Button btnNextOne;
        private EVTechnology.Controls.EVTreeView tvPreview;
        private System.Windows.Forms.ContextMenuStrip cmsMenu;
        private System.Windows.Forms.ToolStripMenuItem tsmRename;
        private System.Windows.Forms.ToolStripMenuItem tsmDeleteRecord;
        private System.Windows.Forms.ToolStripMenuItem tsmDeleteProject;
		private System.Windows.Forms.Label label8;
		private System.Windows.Forms.Label label9;
		private System.Windows.Forms.Label label11;
		private System.Windows.Forms.ListView ivComponent;
        private System.Windows.Forms.Button button1;
	}
}