using System;

namespace EVClassLib
{
    [Serializable]
    public class SSlider : SControl
    {
        public int MaxValue { set; get; } = 100;

        public int MinValue { set; get; } = 0;

        public int Value { set; get; }

        public SSlider()
        {
            this.Type = UIType.SLIDER;
            Event = new ValueEvent();
        }

        public override string GetCode()
        {
            string text = "";
            return text;
        }
    }
}
