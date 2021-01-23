using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ITS.Maraspin.EsercizioPreEsame.FileMessage.Models
{
    public class MessageFileModel
    {
        public string FileName { get; set; }
        public string Message { get; set; }

        public void WriteFile(string fileName, string message)
        {
            var path = @"C:\Temp\" + fileName;
            using (System.IO.StreamWriter file = new System.IO.StreamWriter(path))
            {
                file.WriteLine(message);
            }
        }
    }
}
