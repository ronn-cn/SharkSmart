using System;
using System.Drawing;

namespace EVClassLib
{
    [Serializable]
    public class SText : SControl
    {
        public string Text { set; get; }

        public Color TextColor { set; get; }

        public Color BackColor { set; get; }

        public int FontSize { set; get; }

        public ContentAlignment TextAlign { set; get; } = ContentAlignment.MiddleCenter;

        public SText()
        {
            this.Type = UIType.TEXT;
            Event = new ValueEvent();
        }

        public override string GetCode()
        {
            string text = "";
            if (!string.IsNullOrEmpty(this.ParentName))
            {
                text += String.Format("{0} = SHARK_TEXT_CREATE({1}, {2}, {3}, {4}, {5}, WM_CF_SHOW, 0, GUI_ID_{6}, {7});",
                    this.Name, this.Location.X, this.Location.Y, this.Size.Width, this.Size.Height, this.ParentName, this.Name, "\"" + this.Text + "\"");
                text += string.Format("TEXT_SetFont({0}, &XBF_Font{1});\r", Name, FontSize);
            }
            return text;
        }

        protected string ConverString(string text)
        {
            return text.Replace("\"", "\\\"").Replace("\n", @"\n");
        }

        protected string GetAlignCode()
        {
            string hstr = "";
            switch (TextAlign)
            {
                case ContentAlignment.TopLeft:
                    hstr = "TEXT_CF_TOP|TEXT_CF_LEFT ";
                    break;
                case ContentAlignment.TopCenter:
                    hstr = "TEXT_CF_TOP|TEXT_CF_HCENTER";
                    break;
                case ContentAlignment.TopRight:
                    hstr = "TEXT_CF_TOP|TEXT_CF_RIGHT";
                    break;
                case ContentAlignment.MiddleLeft:
                    hstr = "TEXT_CF_VCENTER|TEXT_CF_LEFT";
                    break;
                case ContentAlignment.MiddleCenter:
                    hstr = "TEXT_CF_VCENTER|TEXT_CF_HCENTER";
                    break;
                case ContentAlignment.MiddleRight:
                    hstr = "TEXT_CF_VCENTER|TEXT_CF_RIGHT";
                    break;
                case ContentAlignment.BottomLeft:
                    hstr = "TEXT_CF_BOTTOM|TEXT_CF_LEFT";
                    break;
                case ContentAlignment.BottomCenter:
                    hstr = "TEXT_CF_BOTTOM|TEXT_CF_HCENTER";
                    break;
                case ContentAlignment.BottomRight:
                    hstr = "TEXT_CF_BOTTOM|TEXT_CF_RIGHT";
                    break;
            }
            return hstr;
        }
    }
}
