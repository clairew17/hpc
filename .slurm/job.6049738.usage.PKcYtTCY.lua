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
  ["currentEpoch"] = 1447524959.0281,
  ["cwd"] = "/home1/03752/tg830512/hpc/hw7",
  ["exec"] = "/home1/03752/tg830512/hpc/hw7/particles",
  ["execEpoch"] = 1447524955,
  ["execModify"] = "Sat Nov 14 12:15:55 2015",
  ["execType"] = "user:binary",
  ["jobID"] = "6049738",
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
  ["runTime"] = "0.0",
  ["runTimeC"] = 1.0280599594116,
  ["sha1"] = "7f0527e04be6e9fd8dcfe16ade0da05fe57d4c4d",
  sizeT = {
    ["bss"] = "1216",
    ["data"] = "4384",
    ["text"] = "89679",
  },
  ["startEpoch"] = "1447524958",
  ["startTime"] = "Sat Nov 14 12:15:58 2015",
  ["user"] = "tg830512",
}