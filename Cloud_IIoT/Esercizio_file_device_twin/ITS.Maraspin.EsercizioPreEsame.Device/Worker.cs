using ITS.Maraspin.EsercizioPreEsame.Device.Models;
using ITS.Maraspin.EsercizioPreEsame.Device.Services;
using ITS.Maraspin.EsercizioPreEsame.Application.Service_Device;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Hosting;
using Microsoft.Extensions.Logging;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;

namespace ITS.Maraspin.EsercizioPreEsame.Device
{
    public class Worker : BackgroundService
    {
        private readonly ILogger<Worker> _logger;
        private readonly IConfiguration _configuration;
        private readonly string _connectionString;
        private string _fileName;
        private string _message;
        private FileModel model = new FileModel();
        private readonly IDeviceService _service;
        private readonly ISetDeviceConfiguration _setDeviceConfiguration;

        public Worker(ILogger<Worker> logger, IConfiguration configuration, IDeviceService service, ISetDeviceConfiguration setDeviceConfiguration)
        {
            _logger = logger;
            _configuration = configuration;
            _service = service;
            _setDeviceConfiguration = setDeviceConfiguration;
            _connectionString = _configuration.GetConnectionString("device");
        }

        protected override async Task ExecuteAsync(CancellationToken stoppingToken)
        {
            _service.InitClient(_connectionString);
            var result = await _service.GetDeviceTwin();
            if(result != null)
            {
                Console.WriteLine("Il desired del dispositivo e {0}", result);
                Console.WriteLine("Vuoi riutilizzare l'ultimo file? [Y / N]");
                var key = (Console.ReadKey()).KeyChar;
                switch (key)
                {
                    case 'y':
                        _fileName = result;
                        break;
                    case 'n':
                        Console.WriteLine("Inserisci il nome del file");
                        _fileName = Console.ReadLine();
                        _setDeviceConfiguration.SetDeviceDesired(_fileName);
                        break;
                }
            }

            while (!stoppingToken.IsCancellationRequested)
            {
              
                Console.WriteLine("Inserisci il messaggio da inviare");
                _message = Console.ReadLine();

                model.FileName = _fileName;
                model.Message = _message;

                await _service.sendMessage(model);
                Console.ReadKey();

                //blocca l'esecuzione del worker finché non viene prmemuto un tasto
           
            }
        }
    }
}
