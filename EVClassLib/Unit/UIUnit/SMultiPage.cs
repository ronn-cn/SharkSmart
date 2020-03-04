using EVTechnology.Common.Controls;
using System;
using System.Collections.Generic;

namespace EVClassLib
{
    [Serializable]
    public class SMultiPage : SControl
    {
        public List<UIPageInfo> Pages { set; get; }

        public bool IsTop { set; get; }

        public bool IsLeft { set; get; }

        public SMultiPage()
        {
            this.Type = UIType.MULTIPAGE;
            Event = new ValueEvent();
        }

        public override string GetCode()
        {
            string text = "";
            return text;
        }
    }
}
