using System;

namespace EVClassLib
{
    [Serializable]
    public class SScrollBar : SControl
    {
        public int Value { set; get; }

        public bool IsH { set; get; } = true;

        public SScrollBar()
        {
            this.Type = UIType.SCROLLBAR;
            Event = new ValueEvent();
        }

        public override string GetCode()
        {
            string text = "";
            return text;
        }
    }
}
