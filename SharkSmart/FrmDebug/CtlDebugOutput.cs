using EVTechnology.Common.Logging;
using System;
using System.Drawing;
using System.Windows.Forms;

namespace SharkSmart
{
    public delegate void UpdateText(Color color, string mq);
    public partial class CtlDebugOutput : UserControl
    {
        private UpdateText updateText;
        public Logger Logger { get; set; }
        public CtlDebugOutput()
        {
            InitializeComponent();
        }

        public CtlDebugOutput(Logger logger) : this()
        {
            this.Logger = logger;
            this.Logger.OutputStreamEvent += new OutputStreamHandler(LoggerOutputStream);
        }

        private void UpdateTextRtbConsole(Color color, string text)
        {
            if (this.rtbConsole.Lines.Length > 200)
            {
                this.rtbConsole.Select(0, this.rtbConsole.Lines[0].Length + 1);
                this.rtbConsole.SelectedText = "";
            }
            this.rtbConsole.Select(this.rtbConsole.Text.Length, 0);
            this.rtbConsole.SelectionColor = color;
            this.rtbConsole.AppendText(text);
            this.rtbConsole.ScrollToCaret();
        }

        private void LoggerOutputStream(string text)
        {
            if (!this.rtbConsole.Created)
                return;

            if (text.Contains("ERROR"))
            {
                this.rtbConsole.BeginInvoke(updateText, Color.Red, text);
            }
            else if (text.Contains("WARN"))
            {
                this.rtbConsole.BeginInvoke(updateText, Color.Yellow, text);
            }
            else
            {
                this.rtbConsole.BeginInvoke(updateText, Color.White, text);
            }
        }

        private void CtlDebugOutput_Load(object sender, EventArgs e)
        {
            updateText += new UpdateText(UpdateTextRtbConsole);
        }
    }
}
