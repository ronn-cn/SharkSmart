using System;
using System.Drawing;

namespace EVClassLib
{
    [Serializable]
    public abstract class SControl : UIBase
    {
        public string Name { get; set; }

        public string ParentName { get; set; }

        public BaseEvent Event { set; get; }

        public UIType Type { get; set; }

        public abstract string GetCode();

        protected string ColorToABGR(Color color)
        {
            byte a, r, g, b;
            a = color.A;
            r = color.R;
            g = color.G;
            b = color.B;
            return Covering(a) + Covering(r) + Covering(g) + Covering(b);
        }

        /// <summary>
        /// 补位
        /// </summary>
        protected string Covering(byte a)
        {
            return a.ToString("X").PadLeft(2, '0');
        }
    }
}
