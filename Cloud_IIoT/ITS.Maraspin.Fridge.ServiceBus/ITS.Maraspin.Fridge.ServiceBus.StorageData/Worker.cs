using ITS.Maraspin.Fridge.ServiceBus.Service;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Hosting;
using Microsoft.Extensions.Logging;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;

namespace ITS.Maraspin.Fridge.ServiceBus.StorageData
{
    public class Worker : BackgroundService
    {
        private readonly ILogger<Worker> _logger;
        private readonly IConfiguration _configuration;
        private readonly string _connectionString;
        private readonly string _connectionStringStorage;

        public Worker(ILogger<Worker> logger, IConfiguration configuration)
        {
            _logger = logger;
            _configuration = configuration;
            _connectionString = _configuration.GetConnectionString("bus");
            _connectionStringStorage = _configuration.GetConnectionString("storage");
        }

        protected override async Task ExecuteAsync(CancellationToken stoppingToken)
        {
            BusService service = new BusService(_connectionString, _connectionStringStorage);
            service.Receive("storage");
        }
    }
}
