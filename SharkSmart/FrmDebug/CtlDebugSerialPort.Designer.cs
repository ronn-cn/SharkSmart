namespace SharkSmart
{
	partial class CtlDebugSerialPort
	{
		/// <summary> 
		/// 必需的设计器变量。
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary> 
		/// 清理所有正在使用的资源。
		/// </summary>
		/// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region 组件设计器生成的代码

		/// <summary> 
		/// 设计器支持所需的方法 - 不要修改
		/// 使用代码编辑器修改此方法的内容。
		/// </summary>
		private void InitializeComponent()
		{
			this.components = new System.ComponentModel.Container();
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(CtlDebugSerialPort));
			this.toolStripNoFrame1 = new EVTechnology.Common.Controls.ToolStripNoFrame();
			this.toolStripLabel4 = new System.Windows.Forms.ToolStripLabel();
			this.tsbOpenCom = new System.Windows.Forms.ToolStripButton();
			this.tscbBaudRate = new System.Windows.Forms.ToolStripComboBox();
			this.toolStripLabel1 = new System.Windows.Forms.ToolStripLabel();
			this.tscbPort = new System.Windows.Forms.ToolStripComboBox();
			this.toolStripLabel2 = new System.Windows.Forms.ToolStripLabel();
			this.rtbConsole = new System.Windows.Forms.RichTextBox();
			this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
			this.toolStripNoFrame1.SuspendLayout();
			this.SuspendLayout();
			// 
			// toolStripNoFrame1
			// 
			this.toolStripNoFrame1.AutoSize = false;
			this.toolStripNoFrame1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(41)))), ((int)(((byte)(44)))), ((int)(((byte)(47)))));
			this.toolStripNoFrame1.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
			this.toolStripNoFrame1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripLabel4,
            this.tsbOpenCom,
            this.tscbBaudRate,
            this.toolStripLabel1,
            this.tscbPort,
            this.toolStripLabel2});
			this.toolStripNoFrame1.Location = new System.Drawing.Point(0, 0);
			this.toolStripNoFrame1.Name = "toolStripNoFrame1";
			this.toolStripNoFrame1.RenderMode = System.Windows.Forms.ToolStripRenderMode.System;
			this.toolStripNoFrame1.Size = new System.Drawing.Size(928, 30);
			this.toolStripNoFrame1.TabIndex = 7;
			this.toolStripNoFrame1.Text = "toolStripNoFrame1";
			// 
			// toolStripLabel4
			// 
			this.toolStripLabel4.Font = new System.Drawing.Font("Microsoft YaHei UI", 10F);
			this.toolStripLabel4.ForeColor = System.Drawing.SystemColors.ButtonFace;
			this.toolStripLabel4.Margin = new System.Windows.Forms.Padding(5, 1, 0, 2);
			this.toolStripLabel4.Name = "toolStripLabel4";
			this.toolStripLabel4.Size = new System.Drawing.Size(65, 27);
			this.toolStripLabel4.Text = "串口通讯";
			// 
			// tsbOpenCom
			// 
			this.tsbOpenCom.Alignment = System.Windows.Forms.ToolStripItemAlignment.Right;
			this.tsbOpenCom.BackColor = System.Drawing.Color.DimGray;
			this.tsbOpenCom.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
			this.tsbOpenCom.Font = new System.Drawing.Font("Microsoft YaHei UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.tsbOpenCom.ForeColor = System.Drawing.SystemColors.ButtonHighlight;
			this.tsbOpenCom.Image = ((System.Drawing.Image)(resources.GetObject("tsbOpenCom.Image")));
			this.tsbOpenCom.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.tsbOpenCom.Name = "tsbOpenCom";
			this.tsbOpenCom.Size = new System.Drawing.Size(60, 27);
			this.tsbOpenCom.Text = "打开串口";
			this.tsbOpenCom.Click += new System.EventHandler(this.tsbOpenCom_Click);
			// 
			// tscbBaudRate
			// 
			this.tscbBaudRate.Alignment = System.Windows.Forms.ToolStripItemAlignment.Right;
			this.tscbBaudRate.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(50)))), ((int)(((byte)(50)))), ((int)(((byte)(50)))));
			this.tscbBaudRate.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.tscbBaudRate.ForeColor = System.Drawing.SystemColors.ButtonFace;
			this.tscbBaudRate.Items.AddRange(new object[] {
            "9600",
            "19200",
            "38400",
            "57600",
            "115200"});
			this.tscbBaudRate.Name = "tscbBaudRate";
			this.tscbBaudRate.Size = new System.Drawing.Size(121, 30);
			this.tscbBaudRate.Text = "115200";
			// 
			// toolStripLabel1
			// 
			this.toolStripLabel1.Alignment = System.Windows.Forms.ToolStripItemAlignment.Right;
			this.toolStripLabel1.ForeColor = System.Drawing.SystemColors.ButtonFace;
			this.toolStripLabel1.Name = "toolStripLabel1";
			this.toolStripLabel1.Size = new System.Drawing.Size(56, 27);
			this.toolStripLabel1.Text = "波特率：";
			// 
			// tscbPort
			// 
			this.tscbPort.Alignment = System.Windows.Forms.ToolStripItemAlignment.Right;
			this.tscbPort.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(50)))), ((int)(((byte)(50)))), ((int)(((byte)(50)))));
			this.tscbPort.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.tscbPort.ForeColor = System.Drawing.SystemColors.ButtonFace;
			this.tscbPort.Name = "tscbPort";
			this.tscbPort.Size = new System.Drawing.Size(121, 30);
			// 
			// toolStripLabel2
			// 
			this.toolStripLabel2.Alignment = System.Windows.Forms.ToolStripItemAlignment.Right;
			this.toolStripLabel2.ForeColor = System.Drawing.SystemColors.ButtonFace;
			this.toolStripLabel2.Name = "toolStripLabel2";
			this.toolStripLabel2.Size = new System.Drawing.Size(56, 27);
			this.toolStripLabel2.Text = "端口号：";
			// 
			// rtbConsole
			// 
			this.rtbConsole.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(59)))), ((int)(((byte)(62)))), ((int)(((byte)(64)))));
			this.rtbConsole.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.rtbConsole.Dock = System.Windows.Forms.DockStyle.Fill;
			this.rtbConsole.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.rtbConsole.ForeColor = System.Drawing.SystemColors.ButtonFace;
			this.rtbConsole.Location = new System.Drawing.Point(0, 30);
			this.rtbConsole.Name = "rtbConsole";
			this.rtbConsole.ReadOnly = true;
			this.rtbConsole.Size = new System.Drawing.Size(928, 722);
			this.rtbConsole.TabIndex = 8;
			this.rtbConsole.Text = "";
			// 
			// CtlDebugSerialPort
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.rtbConsole);
			this.Controls.Add(this.toolStripNoFrame1);
			this.Name = "CtlDebugSerialPort";
			this.Size = new System.Drawing.Size(928, 752);
			this.Load += new System.EventHandler(this.CtlDebugSerialPort_Load);
			this.toolStripNoFrame1.ResumeLayout(false);
			this.toolStripNoFrame1.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion

		private EVTechnology.Common.Controls.ToolStripNoFrame toolStripNoFrame1;
		private System.Windows.Forms.ToolStripLabel toolStripLabel4;
		private System.Windows.Forms.RichTextBox rtbConsole;
		private System.Windows.Forms.ToolStripButton tsbOpenCom;
		private System.Windows.Forms.ToolStripComboBox tscbBaudRate;
		private System.Windows.Forms.ToolStripLabel toolStripLabel1;
		private System.Windows.Forms.ToolStripComboBox tscbPort;
		private System.Windows.Forms.ToolStripLabel toolStripLabel2;
		private System.IO.Ports.SerialPort serialPort1;
	}
}
