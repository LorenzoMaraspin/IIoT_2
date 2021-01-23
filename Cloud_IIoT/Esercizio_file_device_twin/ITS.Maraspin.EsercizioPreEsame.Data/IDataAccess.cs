using System;
using System.Collections.Generic;
using System.Text;
using ITS.Maraspin.EsercizioPreEsame.Data.Models;

namespace ITS.Maraspin.EsercizioPreEsame.Data
{
    public interface IDataAccess
    {
        void Insert(FileModelDb model, string _connectionString);
        IEnumerable<FileModelDb> GetAll(string _connectionString);
        FileModelDb GetById(int id, string _connectionString);
    }
}
