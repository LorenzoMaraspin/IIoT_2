using Microsoft.Extensions.Hosting;
using Microsoft.Extensions.Logging;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using ITS.Maraspin.Fridge.ServiceBus.Model;
using ITS.Maraspin.Fridge.ServiceBus.Service;
using Microsoft.Extensions.Configuration;

namespace ITS.Maraspin.Fridge.ServiceBus.ManageAlert
{
    public class Worker : BackgroundService
    {
        private readonly ILogger<Worker> _logger;
        private readonly IConfiguration _configuration;
        private readonly string _connectionString;

        public Worker(ILogger<Worker> logger, IConfiguration configuration)
        {
            _logger = logger;
            _configuration = configuration;
            _connectionString = _configuration.GetConnectionString("bus");
        }

        protected override async Task ExecuteAsync(CancellationToken stoppingToken)
        {
            BusService service = new BusService(_connectionString);
            service.Receive("altert");
        }
    }
}
