using SharkCompiler.Clang;
using System;
using System.Collections.Generic;
using System.Drawing;

namespace EVClassLib
{
    [Serializable]
    public class UIUnit : Unit
    {
        public List<ViewPage> Pages { set; get; } = new List<ViewPage>();

        public UIUnit(string name) : base(name)
        {
            this.Type = UnitType.UI;
        }

        public UIUnit(string name, int light, int direction, Size dpi) : this(name)
        {
            Pages = new List<ViewPage>();
            Light = light;
            Direction = direction;
            DPI = dpi;

            ViewPage page = new ViewPage();
            page.Name = "Page0";
            Pages.Add(page);
        }

        public int Light { set; get; }

        /// <summary>
        /// 界面单元的显示方向，共4个方向 0：0°  1：90°  2：180°  3：270°
        /// </summary>
        public int Direction { set; get; }

        /// <summary>
        /// 显示的分辨率
        /// </summary>
        public Size DPI { set; get; }

        /// <summary>
        /// 是否显示鼠标
        /// </summary>
        public bool IsShowCursor { get; set; }
        public override string TransformToClang_forC()
        {
            string text = @"#include ""GLOBAL.h""
uint32_t* TASKVIEW_STK;
void TaskView(void* p_arg);
";
            if (Data != null)
            {
                ClangFunctionImplementation imp = Data.FunctionImps.Find(p => p.Reference.Name == Data.Name + "_Init");
                ClangFunctionDefintion def;
                List<ClangStatement> liststmts = new List<ClangStatement>();
                foreach (ClangStatement s in Data.Statements)
                {
                    if (s.GetType() == typeof(ClangAssignmentStatement))
                    {
                        ClangAssignmentStatement cassstmt = s as ClangAssignmentStatement;
                        if (cassstmt.Left.GetType() == typeof(ClangMemberExpression) &&
                            cassstmt.Right.GetType() == typeof(ClangCallExpression))
                        {
                            ClangMemberExpression cleft = cassstmt.Left as ClangMemberExpression;
                            ClangCallExpression cright = cassstmt.Right as ClangCallExpression;
                            string[] list = ((ClangNameExpression)(cleft.Target)).Name.Split('_');
                            if (list == null || list.Length < 2) continue;
                            ViewPage page = Pages.Find(p => p.Name == list[0]);
                            if (page != null)
                            {
                                SControl ctrl = page.Controls.Find(p => p.Name == ((ClangNameExpression)(cleft.Target)).Name);
                                ctrl.Event = new BaseEvent();
                                switch (cleft.Name)
                                {
                                    case "Clicked":
                                        ctrl.Event.Clicked = ((ClangNameExpression)(cright.Target)).Name + "()";
                                        break;
                                }
                                continue;
                            }
                        }
                    }
                    liststmts.Add(s);
                }
                if (imp == null)
                {
                    def = new ClangFunctionDefintion(Data.Name + "_Init", "void", new List<ClangVariable>());
                    imp = new ClangFunctionImplementation(def);
                    imp.Body = new ClangProcedure(liststmts);
                    Data.FunctionDefs.Add(def);
                    Data.FunctionImps.Add(imp);
                }
                else
                {
                    imp.Body.Statements.InsertRange(0, liststmts);
                }

                string str0 = @"
    TASKVIEW_STK = mymalloc(SRAMEX, 512 * 4);
    OSTaskCreate(TaskView,""TaskView"", (void*)0, (uint32_t*)&TASKVIEW_STK[511], EMWINDEMO_TASK_PRIO_NUM);";
                ClangOtherStatement other = new ClangOtherStatement(str0);
                imp.Body.Statements.Add(other);
                text += Data.ToClangData().CreateCode_forC();
            }
            text += @"
void TaskView(void* p_arg)
{";
            for (int i = 0; i < this.Pages.Count; i++)
            {
                text += "    Create" + Pages[i].Name + "();\r";
            }
            text += @"  ShowPage(" + Pages[0].Name + @");
    while (1)
    {
		GuiTouchExec();
        OSTimeDly(15);
    }
}";
            return text;
        }

        public override string TransformToClang_forH()
        {
            string text = "extern void DisplayUnit_Init();\r";
            return text;
        }
    }
}
