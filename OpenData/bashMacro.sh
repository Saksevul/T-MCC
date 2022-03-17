# 0.- Guardamos la fecha y hora de inicio del macro.
start_date=$(date +%x) && start_time=$(date +%X)


# 1.- Definir los parámetros de entrada.

# Ruta al Macro Maestro (RutaMM).
RutaMM=/home/saksevul/T-MCC/OpenData/Extraccion.C
# Ruta a la Lista de AOD's. (RutaLAOD).
RutaLAOD=/home/saksevul/T-MCC/OpenData/ListaAOD
  # RutaLAOD=/home/saksevul/CMS_1.3.0/CMS_Run2011A/Jet_20000/ListaAOD
  # RutaLAOD=/media/saksevul/Pawahtun/CMS_Run2011A/BTag_20000/ListaAOD
# Ruta al Directorio de Archivos de Salida (RutaDAS).
RutaDAS=/home/saksevul/T-MCC/OpenData/Jet_20000/Jets
  # RutaDAS=/home/saksevul/T-MCC/OpenData/BTag_20000/Range
# inicial Tipo de AOD (iTAOD).
iTAOD=Jet
  iTAOD=Jet
  # previo Tipo de AOD (pTAOD).
  pTAOD=$iTAOD
# inicial AOD (iAOD).
iAOD=0001
  # previo AOD (pAOD).
  pAOD=$iAOD


# 2.-
# Ciclo for sobre Tipo de AOD (TAOD).
for TAOD in MultiJet Jet
do
  # Variables para conocer los tiempos de cómputo.
  # start_date_i=$(date +%x) && start_time_i=$(date +%X)
  # Modificamos el Master Macro (MM) para utilizar el TAOD actual.
  sed -i "s/TAOD = \"$pTAOD\"/TAOD = \"$TAOD\"/g" $RutaMM
  # Modificamos RutaDAS para utilizar el TAOD actual.
  RutaDAS=$(echo $RutaDAS | sed "s/$pTAOD\_20000/$TAOD\_20000/g")
    # Ciclo for sobre los archivos en root Files List (AODL).
    for AOD in $(cat $RutaLAOD)
    do
      # Cambiar el root File de entrada en el Master Macro (MM).
      sed -i "s/nAOD = \"$pAOD\"/nAOD = \"$AOD\"/g" $RutaMM
      # Correr el Master Macro (MM). (Sin mensajes de error).
      root -b -l -q 2> /dev/null $RutaMM
      # Redefinimos pAOD para el siguiente ciclo.
      pAOD=$AOD
    done  # Fin del ciclo for para AOD.
  echo $RutaDAS
  # Eliminamos los archivos viejos, pues serán remplazados.
  rm $RutaDAS/$TAOD.root
  # Creamos un único archivo root para el ARJ actual.
  hadd $RutaDAS/$TAOD.root $RutaDAS/0*.root
  # Eliminamos permanentemente los archivos individuales.
  rm $RutaDAS/0*.root
  # Redefinimos pTAOD para el siguiente ciclo.
  pTAOD=$TAOD
  # Término del i-esimo tiempo de cómputo
  # printf "\n\n El Tipo de AOD $TAOD \n\n Inició  el  $start_date_i, a las  $start_time_i.\n\n Terminó el  $(date +%x), a las  $(date +%X).\n\n"
done  # Fin del ciclo for para TAOD.


# 3.- Regresar el Master Macro (MM) a su estado inicial.
sed -i "s/TAOD = \"$pTAOD\"/TAOD = \"$iTAOD\"/g" $RutaMM
sed -i "s/nAOD = \"$pAOD\"/nAOD = \"$iAOD\"/g" $RutaMM


# 4.- Mostrar la fecha y hora de inicio y término de este macro.
printf "\n\n Inició  el  $start_date, a las  $start_time\n\n Terminó el  $(date +%x), a las  $(date +%X)\n\n"


## 5.- Eliminar todas las variables utilizadas.
exec bash
