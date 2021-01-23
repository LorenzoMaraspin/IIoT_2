using ITS.Maraspin.EsercizioPreEsame.Data.Models;
using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using Dapper.Contrib;
using Dapper;
using Dapper.Contrib.Extensions;

namespace ITS.Maraspin.EsercizioPreEsame.Data
{
    public class DataAccess : IDataAccess
    {
        public DataAccess()
        {
        }
        public IEnumerable<FileModelDb> GetAll(string _connectionString)
        {
            using (var connection = new SqlConnection(_connectionString))
            {
                return connection.GetAll<FileModelDb>();
            }
        }

        public FileModelDb GetById(int id, string _connectionString)
        {
            using (var connection = new SqlConnection(_connectionString))
            {
                return connection.Get<FileModelDb>(id);
            }
        }

        public void Insert(FileModelDb model, string _connectionString)
        {
            using (var connection = new SqlConnection(_connectionString))
            {
                connection.Insert(model);  
            }
        }
    }
}
