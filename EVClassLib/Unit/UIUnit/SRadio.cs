using System;
using System.Collections.Generic;

namespace EVClassLib
{
    [Serializable]
    public class SRadio : SControl
    {
        public List<string> Options { set; get; }

        public int Distance { set; get; }

        public SRadio()
        {
            this.Type = UIType.RADIO;
            Event = new ValueEvent();
            Options = new List<string>();
        }

        public override string GetCode()
        {
            string text = "";
            return text;
        }
    }
}
