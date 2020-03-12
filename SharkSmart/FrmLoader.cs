using System;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SharkSmart
{
    /// <summary>
    /// 暂时保留 未做加载
    /// </summary>
    public partial class FrmLoader : Form
    {
        private int _value;

        public FrmLoader()
        {
            InitializeComponent();
        }

        private void FrmLoader_Activated(object sender, EventArgs e)
        {
            CancellationTokenSource cts = new CancellationTokenSource();
            CancellationToken token = cts.Token;
            Task.Run(() =>
            {
                int index = 0;
                while (true)
                {
                    if (cts.Token.IsCancellationRequested)//如果检测到取消请求
                    {
                        return;
                    }
                    var x = this.BeginInvoke(new Action(() =>
                    {
                        this.progressBar1.Value = _value;
                        if (this.progressBar1.Value >= 100)
                        {
                            this.Hide();
                            FrmStartPage f2 = new FrmStartPage();
                            f2.Show();
                            if (cts != null)
                            {
                                cts.Cancel();
                            }
                        }
                    }));
                    this.EndInvoke(x);
                    index += 30;
                    if (index > 30000)
                    {
                        this.Close();
                    }
                    Thread.Sleep(100);
                }
            }, token);

            Task.Run(() =>
            {
                Thread.Sleep(500);
                this._value = 50;
                Thread.Sleep(500);
                this._value = 100;
            });
        }
    }
}
