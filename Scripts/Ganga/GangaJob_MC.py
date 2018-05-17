
#================================================#
#=== GANGA SUBMISSION SCRIPT FOR MONTE CARLO  ===#
#================================================#
#=== Automatically submits jobs for both      ===#
#=== polarities, and can run over multiple    ===#
#=== years and modes.                         ===#
#=== davinci.py must be given the year and    ===#
#=== mag values, as well as whether we are    ===#
#=== looking at MC or data. Seen as though    ===#
#=== we are considering MC here, the data tag ===#
#=== is set to "MC".                          ===#
#=== The options file must also be told about ===#
#=== the mode being considered, as it needs   ===#
#=== this information in order to set DB tags ===#
#================================================#

#=== Set which data you want to run over (years, polarities, modes) ===#
polarities	= ['Down',
              'Up'
               ]
years		= [# '2011'
               '2012'
              , '2015'
              , '2016'
               ]
modes       = ["DK_D2KPiPi0", #"DK_D2KKPi0",  "DK_D2PiPiPi0",
               "DPi_D2KPiPi0", #"DPi_D2KKPi0", "DPi_D2PiPiPi0"
               ]

#=== This should not be changed as this script is used exclusively for submission of MC ===#
#=== as the directories are different for MC compared to data                           ===#
data="MC"

for year in years:
    for mag in polarities:
        for mode in modes:
    
            print "Preparing job for "+year+" Mag" + mag+" with mode: "+mode
            
            #===============================================================#
            #=== Set up job for given year / polarity / mode combination ===#
            #===============================================================#
            
            #=== Write descriptive and unique file name ===#
            #=== e.g. MC_Bu2DK_D2KPiPi0_2012_MagUp.root ===#
            outputfile = 'MC_Bu2'+mode+'_'+year+'_Mag'+mag+'.root'
            summaryfile = 'summary.xml'

            #=== Create a job instance and name with polarity and year ===#
            j=Job()
            j.name=mag+'_'+year
            
            #=== Comment gives information about mode being considered ===#
            j.comment= "MC_Bu2"+mode
    
            myApp = GaudiExec()
            myApp.directory = "/home/colmmurphy/cmtuser/DaVinciDev_v42r6p1"
            j.application = myApp
            j.application.options = ['/home/colmmurphy/davinci.py']
            
            #=== Give extra options to the davinci.py options files ===#
            j.application.extraOpts = (
                "import sys, os\n"
                "sys.path.append(os.getcwd())\n"
                "from davinci import options\n"
                "options('{y}','{m}','{d}','{c}')\n"
            ).format(
                y=year,
                m=mag,
                d=data,
                c=mode
            )
       
            #=================================================================#
            #=== Give correct data path depending on mode being considered ===#
            #=================================================================#
            if year == '2011':
                if mode == "DK_D2KPiPi0":
                    dir = "/MC/2011/Beam3500GeV-2011-Mag"+mag+"-Nu2-Pythia8/Sim09c/Trig0x40760037/Reco14c/Stripping21r1NoPrescalingFlagged/12163441/ALLSTREAMS.DST"
                elif mode == "DPi_D2KPiPi0":
                    dir = "/MC/2011/Beam3500GeV-2011-Mag"+mag+"-Nu2-Pythia8/Sim09c/Trig0x40760037/Reco14c/Stripping21r1NoPrescalingFlagged/12163451/ALLSTREAMS.DST"
                else:
                    print "Error: mode not correct!"
            if year == '2012':
                if mode == "DK_D2KPiPi0":
                    dir = "/MC/2012/Beam4000GeV-2012-Mag"+mag+"-Nu2.5-Pythia8/Sim09c/Trig0x409f0045/Reco14c/Stripping21NoPrescalingFlagged/12163441/ALLSTREAMS.DST"
                elif mode == "DPi_D2KPiPi0":
                    dir = "/MC/2012/Beam4000GeV-2012-Mag"+mag+"-Nu2.5-Pythia8/Sim09c/Trig0x409f0045/Reco14c/Stripping21NoPrescalingFlagged/12163451/ALLSTREAMS.DST"
                elif mode == "DK_D2KKPi0":
                    dir = "/MC/2012/Beam4000GeV-2012-Mag"+mag+"-Nu2.5-Pythia8/Sim08f/Digi13/Trig0x409f0045/Reco14a/Stripping20NoPrescalingFlagged/12163481/ALLSTREAMS.DST"
                elif mode == "DPi_D2KKPi0":
                    dir = "/MC/2012/Beam4000GeV-2012-Mag"+mag+"-Nu2.5-Pythia8/Sim08f/Digi13/Trig0x409f0045/Reco14a/Stripping20NoPrescalingFlagged/12163483/ALLSTREAMS.DST"
                elif mode == "DK_D2PiPiPi0":
                    dir = "/MC/2012/Beam4000GeV-2012-Mag"+mag+"-Nu2.5-Pythia8/Sim08f/Digi13/Trig0x409f0045/Reco14a/Stripping20NoPrescalingFlagged/12163482/ALLSTREAMS.DST"
                elif mode == "DPi_D2PiPiPi0":
                    dir = "/MC/2012/Beam4000GeV-2012-Mag"+mag+"-Nu2.5-Pythia8/Sim08f/Digi13/Trig0x409f0045/Reco14a/Stripping20NoPrescalingFlagged/12163484/ALLSTREAMS.DST"
                else:
                    print "Error: mode not correct!"
            if year == '2015':
                if mode == "DK_D2KPiPi0":
                    dir = "/MC/2015/Beam6500GeV-2015-Mag"+mag+"-Nu1.6-25ns-Pythia8/Sim09c/Trig0x411400a2/Reco15a/Turbo02/Stripping24r1NoPrescalingFlagged/12163441/ALLSTREAMS.MDST"
                elif mode == "DPi_D2KPiPi0":
                    dir = "/MC/2015/Beam6500GeV-2015-Mag"+mag+"-Nu1.6-25ns-Pythia8/Sim09c/Trig0x411400a2/Reco15a/Turbo02/Stripping24r1NoPrescalingFlagged/12163451/ALLSTREAMS.MDST"
                else:
                    print "Error: mode not correct!"
            if year == '2016':
                if mode == "DK_D2KPiPi0":
                    dir = "/MC/2016/Beam6500GeV-2016-Mag"+mag+"-Nu1.6-25ns-Pythia8/Sim09c/Trig0x6138160F/Reco16/Turbo03/Stripping28r1NoPrescalingFlagged/12163441/ALLSTREAMS.MDST"
                elif mode == "DPi_D2KPiPi0":
                    dir = "/MC/2016/Beam6500GeV-2016-Mag"+mag+"-Nu1.6-25ns-Pythia8/Sim09c/Trig0x6138160F/Reco16/Turbo03/Stripping28r1NoPrescalingFlagged/12163451/ALLSTREAMS.MDST"
                else:
                    print "Error: mode not correct!"
            
            print dir;
            
            #=== Get data from bookkeeping (bk) ===#
            bk = BKQuery(
                dqflag = "OK",
                path = dir,
                type = "Path"
                )

            j.inputdata = bk.getDataset()

            #=== Split job into subjobs ===#
            j.splitter=SplitByFiles(filesPerJob=5, ignoremissing=True)
            j.splitter.maxFiles=-1

            j.outputfiles=[outputfile, summaryfile]
            
            #=== Submit job to the GRID ===#
            j.backend=Dirac()
            queues.add(j.submit)
