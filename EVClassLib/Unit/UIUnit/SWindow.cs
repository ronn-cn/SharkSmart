using System;
using System.Drawing;

namespace EVClassLib
{
    [Serializable]
    public class SWindow : SControl
    {
        /// <summary>
        /// 背景颜色
        /// </summary>
        public Color BackColor { set; get; }

        public SWindow()
        {
            this.Type = UIType.WINDOW;
            this.BackColor = Color.White;
        }

        public override string GetCode()
        {
            string parent = "hParent";
            if (!string.IsNullOrEmpty(this.ParentName))
            {
                parent = this.ParentName;
            }
            string text = String.Format("{0} = CreatePageFormPath({1}, {2}, 0, \"resources/{0}.raw\", {0}_callback);\r",
                Name, Size.Width, Size.Height);
            return text;
        }
    }
}
