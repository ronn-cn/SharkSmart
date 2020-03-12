using EVTechnology.Common.Helper;
using System;
using System.Drawing;

namespace EVClassLib
{
    [Serializable]
    public class SButton : SControl
    {
        public string Text { set; get; }

        public Color TextColor { set; get; }

        public Color BackColor { set; get; }

        public int FontSize { set; get; }

        public ContentAlignment TextAlign { set; get; } = ContentAlignment.MiddleCenter;

        public SButton()
        {
            this.Type = UIType.BUTTON;
        }

        public string XMLImagePath { set; get; }

        public override string GetCode()
        {
            string text = "";
            if (!string.IsNullOrEmpty(this.ParentName))
            {
                string tmp;
                if (string.IsNullOrEmpty(Text))
                {
                    tmp = "NULL";
                }
                else
                    tmp = Name + "_text";

                text = String.Format("  {0} = CreateControl({1}, {2}, {3}, {4}, {5}, {6}, NULL, {7}, {8}, {9}, 0x{10});\r",
                    Name, Location.X, Location.Y, Size.Width, Size.Height, 0, ParentName, tmp, Text.Length, FontSize,
                    DrawHelper.RGB32toRGB16(TextColor));
            }
            return text;
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
