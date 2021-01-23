using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ITS.Maraspin.EsercizioPreEsame.Device.Models;

namespace ITS.Maraspin.EsercizioPreEsame.Device.Services
{
    public interface IDeviceService
    {
        Task sendMessage(FileModel model);
        Task<string> GetDeviceTwin();
        void InitClient(string connectionString);
    }
}
