using System;
using System.Drawing;

namespace EVClassLib
{
    [Serializable]
    public abstract class UIBase
    {
        public Point Location { set; get; }

        public Size Size { set; get; }

        public Font Font { set; get; }
    }
}
