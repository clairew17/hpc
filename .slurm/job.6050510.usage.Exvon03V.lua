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
  ["currentEpoch"] = 1447535966.5323,
  ["cwd"] = "/home1/03752/tg830512/hpc/hw7",
  ["exec"] = "/home1/03752/tg830512/hpc/hw7/particles",
  ["execEpoch"] = 1447535924,
  ["execModify"] = "Sat Nov 14 15:18:44 2015",
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
  ["runTime"] = "1",
  ["runTimeC"] = 1.5322620868683,
  ["sha1"] = "471b9310a1c4ef287944d905cd6c5a05b7e770cc",
  sizeT = {
    ["bss"] = "1216",
    ["data"] = "4384",
    ["text"] = "90215",
  },
  ["startEpoch"] = "1447535965",
  ["startTime"] = "Sat Nov 14 15:19:25 2015",
  ["user"] = "tg830512",
}
