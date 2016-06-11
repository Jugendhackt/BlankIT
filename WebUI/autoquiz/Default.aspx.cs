using System;
using System.Web.UI;
using System.Net;
using System.Text;
using System.IO;
using System.Web;

namespace WebUI
{
    public partial class Default : Page
    {

        protected void SearchButton_Click(object sender, EventArgs e)
        {
            try
            {
                string url = "http://192.168.173.28/index.php";
                string fq = "2";
                HttpWebRequest req = (HttpWebRequest)WebRequest.Create(url);
                req.Method = "POST";
                string Data = "text"+"=" + SearchBox.Text + "&fq=" + fq;
                byte[] postBytes = Encoding.ASCII.GetBytes(Data);
                req.ContentType = "application/x-www-form-urlencoded";
                req.ContentLength = Data.Length;
                Stream requestStream = req.GetRequestStream();
                requestStream.Write(postBytes, 0, postBytes.Length);
                requestStream.Close();

                HttpWebResponse myHttpWebResponse = (HttpWebResponse)req.GetResponse();
                Stream responseStream = myHttpWebResponse.GetResponseStream();
                StreamReader myStreamReader = new StreamReader(responseStream, Encoding.Default);
                string pageContent = myStreamReader.ReadToEnd();
                myStreamReader.Close();
                responseStream.Close();
                myHttpWebResponse.Close();
                ClientScript.RegisterStartupScript(GetType(), "alert", pageContent, true);
            }
            catch (WebException)
            {
            }
        }
    }
}