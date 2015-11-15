userT = {
  ["I_MPI_DAPL_PROVIDER"] = "ofa-v2-mlx4_0-1u",
  ["I_MPI_EXTRA_FILESYSTEM"] = "enable",
  ["I_MPI_EXTRA_FILESYSTEM_LIST"] = "lustre",
  ["I_MPI_FABRICS"] = "shm:dapl",
  ["I_MPI_OFA_ADAPTER_NAME"] = "mlx4_0",
  ["MV2_DEFAULT_TIME_OUT"] = "23",
  ["MV2_HOMOGENEOUS_CLUSTER"] = "1",
  ["MV2_IBA_HCA"] = "mlx4_0",
  ["MV2_USE_HUGEPAGES"] = "0",
  ["MV2_USE_OLD_BCAST"] = "0",
  ["MV2_USE_RING_STARTUP"] = "0",
  ["MV2_USE_UD_HYBRID"] = "0",
  ["account"] = "TG-CCR150026",
  ["currentEpoch"] = 1447536897.025,
  ["cwd"] = "/home1/03752/tg830512/hpc/hw7",
  ["exec"] = "/home1/03752/tg830512/hpc/hw7/particles",
  ["execEpoch"] = 1447536591,
  ["execModify"] = "Sat Nov 14 15:29:51 2015",
  ["execType"] = "user:binary",
  ["jobID"] = "6050510",
  ["numCores"] = "16",
  ["numNodes"] = "1",
  ["numThreads"] = "1",
  pkgT = {
    ["intel/15.0.2"] = {
      flavor = {
        "default",
      },
      libA = {
        "libifport.so.5", "libifcore.so.5", "libimf.so", "libsvml.so", "libintlc.so.5",
      },
    },
    ["mvapich2/2.1"] = {
      flavor = {
        "default:intel/15.0.2",
      },
      libA = {
        "libmpi.so.12",
      },
    },
  },
  ["queue"] = "development",
  ["runTime"] = "2",
  ["runTimeC"] = 2.0249509811401,
  ["sha1"] = "60ac5c2c3588fe1d776cf209d39040d4486a0bcf",
  sizeT = {
    ["bss"] = "1216",
    ["data"] = "4384",
    ["text"] = "90295",
  },
  ["startEpoch"] = "1447536895",
  ["startTime"] = "Sat Nov 14 15:34:55 2015",
  ["user"] = "tg830512",
}
