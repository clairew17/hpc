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
  ["currentEpoch"] = 1447530071.4214,
  ["cwd"] = "/home1/03752/tg830512/hpc/hw7",
  ["exec"] = "/home1/03752/tg830512/hpc/hw7/particles",
  ["execEpoch"] = 1447529359,
  ["execModify"] = "Sat Nov 14 13:29:19 2015",
  ["execType"] = "user:binary",
  ["jobID"] = "6050087",
  ["numCores"] = "64",
  ["numNodes"] = "4",
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
  ["queue"] = "normal",
  ["runTime"] = "5",
  ["runTimeC"] = 5.4213619232178,
  ["sha1"] = "88ca15e53aed7c6f477fb4f0aff41301330e2579",
  sizeT = {
    ["bss"] = "1216",
    ["data"] = "4384",
    ["text"] = "90135",
  },
  ["startEpoch"] = "1447530066",
  ["startTime"] = "Sat Nov 14 13:41:06 2015",
  ["user"] = "tg830512",
}
