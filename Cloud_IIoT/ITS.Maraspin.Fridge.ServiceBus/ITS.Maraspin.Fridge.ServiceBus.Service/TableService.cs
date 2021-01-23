using ITS.Maraspin.Fridge.ServiceBus.Data;
using Microsoft.Azure.Cosmos.Table;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Logging;
using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

namespace ITS.Maraspin.Fridge.ServiceBus.Service
{
    public class TableService
    {
        private readonly string _connectionString;

        public TableService(string connectionString) 
        {
            _connectionString = connectionString;
        }

        public async Task SaveData(FridgeEntity value)
        {
            var account = CloudStorageAccount.Parse(_connectionString);
            var tableClient = account.CreateCloudTableClient(new TableClientConfiguration());
            var table = tableClient.GetTableReference("fridge");
            await table.CreateIfNotExistsAsync();
            TableOperation insertOperation = TableOperation.Insert(value);
            table.ExecuteAsync(insertOperation).ConfigureAwait(false);
        }
    }
}
