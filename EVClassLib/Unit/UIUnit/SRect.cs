using EVTechnology.Common.Helper;
using System;
using System.Drawing;

namespace EVClassLib
{
    [Serializable]
    public class SRect : SControl
    {
        public Color FillColor { set; get; } = Color.DeepSkyBlue;

        public SRect()
        {
            this.Type = UIType.NONE;
        }

        public override string GetCode()
        {
            string text = string.Format("EV_LCD_FillRectangle({0}, {1}, {2}, {3}, {4}, {5});\r", Location.X, Location.Y,
                Size.Width + Location.X, Size.Height + Location.Y, DrawHelper.RGB32toRGB16(FillColor), 0);
            return text;
        }
    }
}
