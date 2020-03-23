namespace SharkSmart
{
	partial class CtlDebugOutput
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
			this.toolStripNoFrame1 = new EVTechnology.Controls.ToolStripNoFrame();
			this.toolStripLabel4 = new System.Windows.Forms.ToolStripLabel();
			this.rtbConsole = new System.Windows.Forms.RichTextBox();
			this.toolStripNoFrame1.SuspendLayout();
			this.SuspendLayout();
			// 
			// toolStripNoFrame1
			// 
			this.toolStripNoFrame1.AutoSize = false;
			this.toolStripNoFrame1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(20)))), ((int)(((byte)(23)))), ((int)(((byte)(51)))));
			this.toolStripNoFrame1.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
			this.toolStripNoFrame1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripLabel4});
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
			this.toolStripLabel4.Size = new System.Drawing.Size(37, 27);
			this.toolStripLabel4.Text = "输出";
			// 
			// rtbConsole
			// 
			this.rtbConsole.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(42)))), ((int)(((byte)(46)))), ((int)(((byte)(77)))));
			this.rtbConsole.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.rtbConsole.Dock = System.Windows.Forms.DockStyle.Fill;
			this.rtbConsole.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.rtbConsole.ForeColor = System.Drawing.SystemColors.ButtonFace;
			this.rtbConsole.Location = new System.Drawing.Point(0, 30);
			this.rtbConsole.Name = "rtbConsole";
			this.rtbConsole.Size = new System.Drawing.Size(928, 722);
			this.rtbConsole.TabIndex = 8;
			this.rtbConsole.Text = "";
			// 
			// CtlDebugOutput
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.rtbConsole);
			this.Controls.Add(this.toolStripNoFrame1);
			this.Name = "CtlDebugOutput";
			this.Size = new System.Drawing.Size(928, 752);
			this.Load += new System.EventHandler(this.CtlDebugOutput_Load);
			this.toolStripNoFrame1.ResumeLayout(false);
			this.toolStripNoFrame1.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion

		private EVTechnology.Controls.ToolStripNoFrame toolStripNoFrame1;
		private System.Windows.Forms.ToolStripLabel toolStripLabel4;
		private System.Windows.Forms.RichTextBox rtbConsole;
	}
}
