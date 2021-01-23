using Dapper.Contrib.Extensions;
using System;
using System.Collections.Generic;
using System.Text;

namespace ITS.Maraspin.EsercizioPreEsame.Data.Models
{
    [Table("tbfile")]
    public class FileModelDb
    {
        public int Id { get; set; }
        public string Filename { get; set; }
        public string Message { get; set; }
    }
}
