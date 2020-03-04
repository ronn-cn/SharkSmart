using System;

namespace EVClassLib
{
    [Serializable]
    public class SProgbar : SControl
    {
        public int Value { set; get; }

        public SProgbar()
        {
            this.Type = UIType.PROGBAR;
            Event = new ValueEvent();
        }

        public override string GetCode()
        {
            string text = "";
            return text;
        }
    }
}
