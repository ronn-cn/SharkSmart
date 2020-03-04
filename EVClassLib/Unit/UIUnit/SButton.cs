using System;

namespace EVClassLib
{
    [Serializable]
    public class SButton : SText
    {
        public SButton()
        {
            this.Type = UIType.BUTTON;
            Event = new BaseEvent();
        }

        public string XMLImagePath { set; get; }

        public override string GetCode()
        {
            string text = "";
            if (!string.IsNullOrEmpty(this.ParentName))
            {
                text = String.Format("{0} = CreateControl({1}, {2}, {3}, {4}, {5}, {6}, NULL, {7}, {8}, 16);\r",
                    Name, Location.X, Location.Y, Size.Width, Size.Height, 0, ParentName, Name + "_text", Text.Length + 1);
            }
            return text;
        }
    }
}
