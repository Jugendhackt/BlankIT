using System;
using System.Web.UI;
using System.Net;
using System.Text;
using System.IO;
using System.Web;
using System.Text.RegularExpressions;
using System.Collections.Generic;
using System.Web.UI.WebControls;

namespace WebUI
{
    public partial class Default : Page
    {
        public const string BLK = "[@22]"; //Blank
        public const string EOL = "[@23]"; //End of line
        public const string EOF = "[@24]"; //End of file


        //Deklaration der Variablen
        public List<string> sentences
        {
            get
            {
                return (List<string>) ViewState["sentences"];
            }
            set
            {
                ViewState["sentences"] = value;
            }
        }
        public List<string> solutions
        {
            get
            {
                return (List<string>)ViewState["solutions"];
            }
            set
            {
                ViewState["solutions"] = value;
            }
        }
        public int currentSentenceIndex
        {
            get
            {
                return (int)ViewState["currentSentenceIndex"];
            }
            set
            {
                ViewState["currentSentenceIndex"] = value;
            }
        }

        public string currentSentence
        { get { return sentences[currentSentenceIndex]; } }

        public string currentSolution
        { get { return solutions[currentSentenceIndex]; } }


        protected void Page_Load(object sender, EventArgs e)
        {

        }
        protected void SearchButton_Click(object sender, EventArgs e)
        {
            //if (LueckenButton.Click == null)
            //LueckenButton.Click += LueckenButton_Click;
            string PageRet = AskData(SearchBox.Text, 1); //PageReteurn

            InitQuest(PageRet);
            putQuestAndIncrease();
            //Debug.Text = solutions.Count + "|" + sentences.Count;
        }
        protected void Lueckentext_Change(object sender, EventArgs e)
        {

        }
        protected string AskData(string Text, int Quatschfaktor)
        {
            /* 
             * Senden eines HTTP POST REQUEST mittels HttpWebRequest zu einer Adresse (url).
             */

            string url = (WikiEnabled.Checked)?"http://192.168.173.28/indexWiki.php":"http://192.168.173.28/index.php";
            string fq = "1";
            HttpWebRequest req = (HttpWebRequest)WebRequest.Create(url);
            req.Method = "POST";
            string Data = "text" + "=" + Text + "&fq=" + fq; //SearchBox.Text
            byte[] postBytes = Encoding.ASCII.GetBytes(Data);
            req.ContentType = "application/x-www-form-urlencoded";
            req.ContentLength = Data.Length;
            Stream requestStream = req.GetRequestStream();
            requestStream.Write(postBytes, 0, postBytes.Length);

            HttpWebResponse res = (HttpWebResponse)req.GetResponse();

            Stream responseStream = res.GetResponseStream();
            StreamReader myStreamReader = new StreamReader(responseStream, Encoding.Default);

            string ret = myStreamReader.ReadToEnd();

            myStreamReader.Close();
            responseStream.Close();
            requestStream.Close();
            res.Close();

            ret.Replace( EOF, "");

            //Debug.Text = ret;
            return ret;
        }

        protected void InitQuest(string PageRet)
        {
            sentences = new List<string>(PageRet.Split(new string[] { EOL }, StringSplitOptions.None));
            sentences.RemoveAt(sentences.Count - 1);
            
            List<string> words = new List<string>(PageRet.Split(new string[] { " " }, StringSplitOptions.None));

            //for (int i = 0; i < words.Count; i++)
            //    Debug.text += sentences[i];

            solutions = new List<string>();

            for (int i = 0; i < words.Count; i++)
                if (words[i].Contains(BLK))
                {
                    solutions.Add(words[i].Replace(BLK, ""));
                }

            currentSentenceIndex = -1;
            //for (int i = 0; i < sentences.Count; i++)
            //    Debug.Text += sentences[i];
            //Debug.Text += "  KEYS:";
            //for (int i = 0; i < solutions.Count; i++)
            //    Debug.Text += solutions[i];
        }
        protected void putQuestAndIncrease()
        {
            currentSentenceIndex++;
            if (currentSentenceIndex == solutions.Count)
                Response.Redirect("~/Success.aspx");
            //Debug.Text = currentSentenceIndex + "";
            Lueckentext.Text = currentSentence.Replace(BLK, "").Replace(currentSolution, "________");

            //Debug.Text = "DEV: " + currentSolution;
        }

        protected void ConfirmInput(object sender, ImageClickEventArgs e)
        {
            if (Lueckenfüller.Text == currentSolution) putQuestAndIncrease();
        }

        protected void LueckenButton_Load(object sender, EventArgs e)
        {
            this.LueckenButton.Click += this.ConfirmInput;
        }
    }
}