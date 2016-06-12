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
        //Deklaration der Variablen
        string pageContent = "";
        HttpWebRequest req;
        HttpWebResponse res;
        

        protected void Page_Load(object sender, EventArgs e)
        {
                       
        }
        protected void SearchButton_Click(object sender, EventArgs e)
        {
            PostRequest(SearchBox.Text, 1);
            pageContent=GetResponse();
            PrintSentences();
        }
        protected void Lueckentext_Change(object sender, EventArgs e) {

        }
        protected void LueckenButton_Click(object sender, EventArgs e) {
            //TextBox txt = (TextBox)Parent.FindControl("Lueckentext");
            //txt.Text.Insert(0, "Bla");
        }
        protected void PrintSentences()
        {
            string blanks = "[@22]";
            string sentenceEnd = "[.]\\[@23\\]";

            if (pageContent.Length >= 5)
            {
                SearchBox.Text = "";
                pageContent = pageContent.Substring(0, pageContent.Length - 5);     //Entfernen der letzten Zeichen
                string[] senten = Regex.Split(pageContent, sentenceEnd);
                List<List<string>> sentences = new List<List<string>>();
                foreach (string s in senten)
                {
                    Console.WriteLine(s);
                    List<string> words = new List<string>();
                    string[] wos = s.Split(' ');
                    foreach (string w in wos)
                    {
                        words.Add(w);
                    }
                    sentences.Add(words);
                }
                sentences.RemoveAt(sentences.Count - 1);
                for (int i = 0; i < sentences.Count; i++)
                {
                    //Lueckentext.Text += sentences.Count + " "+sentences[1].Count + " " + pageContent;
                    if (sentences[i].Count > 2)
                    {
                        for (int j = 0; j < sentences[i].Count; j++)
                        {
                            if (sentences[i][j].Length >= 5 && sentences[i][j].Substring(0, 5) == blanks)
                            {
                                string realWo = sentences[i][j].Substring(5, sentences[i][j].Length - 5);
                                Lueckentext.Text += "-";
                                for (int k = 0; k < realWo.Length; k++)
                                {
                                    Lueckentext.Text += "_";
                                }
                                Lueckentext.Text += "- ";
                            }
                            else
                            {
                                Lueckentext.Text += sentences[i][j] + " ";
                            }
                        }
                        Lueckentext.Text += ".<br/>";
                    }
                }
            }
        }
        protected void PostRequest(string Text, int Quatschfaktor) {
            /* 
             * Senden eines HTTP POST REQUEST mittels HttpWebRequest zu einer Adresse (url).
             */

            string url = "http://192.168.173.28/index.php";
            string fq = "2";
            req = (HttpWebRequest)WebRequest.Create(url);
            req.Method = "POST";
            string Data = "text" + "=" + Text + "&fq=" + fq; //SearchBox.Text
            byte[] postBytes = Encoding.ASCII.GetBytes(Data);
            req.ContentType = "application/x-www-form-urlencoded";
            req.ContentLength = Data.Length;
            Stream requestStream = req.GetRequestStream();
            requestStream.Write(postBytes, 0, postBytes.Length);
            requestStream.Close();
        }
        protected string GetResponse() {
            /*
             * Empfangen eines HTTP GET RESPONSE mittels HttpWebResponse von einer anderen Adresse
             */
            res = (HttpWebResponse)req.GetResponse();
            Stream responseStream = res.GetResponseStream();
            StreamReader myStreamReader = new StreamReader(responseStream, Encoding.Default);
            string t = myStreamReader.ReadToEnd();
            myStreamReader.Close();
            responseStream.Close();
            res.Close();
            ClientScript.RegisterStartupScript(GetType(), "alert", t, true);
            return t;
        }
    }   
}