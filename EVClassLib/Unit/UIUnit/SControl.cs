using System;
using System.Drawing;

namespace EVClassLib
{
    [Serializable]
    public abstract class SControl
    {
        public Point Location { set; get; }

        public Size Size { set; get; }

        public Font Font { set; get; }

        public string Name { get; set; }

        public string ParentName { get; set; }

        public UIType Type { get; set; }

        public string OtherFiled { set; get; }

        public abstract string GetCode();
    }

    public enum UIType
    {
        NONE,
        WINDOW,
        BUTTON,
        TEXT,
        EDIT,
        HEADER,
        MULTIEDIT,
        MULTIPAGE,
        CHECKBOX,
        RADIO,
        PROGBAR,
        ICONVIEW,
        IMAGE,
        SCROLLBAR,
        SLIDER,
    }
}
