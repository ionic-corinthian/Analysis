#years		= ['2011', '2012', '2015', '2016', '2017']

polarities = ['Up', 'Down'] 
years = ['2017']

data='data'

for year in years:
    for mag in polarities:
        print "Preparing job for "+year+" Mag" + mag
        
        outputfile = 'Data_Bu2DH_AllModes_'+year+'_Mag'+mag+'.root'
        summaryfile = 'summary.xml'
        
        j=Job()
        j.name=mag+'_'+year
        
        j.comment= "Data_Bu2DH_AllModes"
        
        myApp = GaudiExec()
        myApp.directory = "/home/colmmurphy/DaVinciDev_v42r6p1/"
        j.application = myApp
        j.application.options = ['/home/colmmurphy/davinci.py']
        j.application.extraOpts = (
            "import sys, os\n"
            "sys.path.append(os.getcwd())\n"
            "from davinci import options\n"
            "options('{y}','{m}','{d}', '{a}')\n"
        ).format(
            y=year,
            m=mag,
            d=data,
            a="a"
        )
        
        if year == "2011":
            dir = "LHCb/Collision11/Beam3500GeV-VeloClosed-Mag"+mag+"/Real Data/Reco14/Stripping21r1/90000000/BHADRON.MDST"
        if year == "2012":
            dir = "LHCb/Collision12/Beam4000GeV-VeloClosed-Mag"+mag+"/Real Data/Reco14/Stripping21/90000000/BHADRON.MDST"
        if year == "2015":
            dir = "LHCb/Collision15/Beam6500GeV-VeloClosed-Mag"+mag+"/Real Data/Reco15a/Stripping24r1/90000000/BHADRON.MDST"
        if year == "2016":
            dir = "LHCb/Collision16/Beam6500GeV-VeloClosed-Mag"+mag+"/Real Data/Reco16/Stripping28r1/90000000/BHADRON.MDST"
        if year == "2017":
            dir = "LHCb/Collision17/Beam6500GeV-VeloClosed-Mag"+mag+"/Real Data/Reco17/Stripping29r1/90000000/BHADRON.MDST"
        
        
        
        bk = BKQuery(
            dqflag = "OK",
            path = dir,
            type = "Path"
            )

        j.inputdata = bk.getDataset()


        j.splitter=SplitByFiles(filesPerJob=3, ignoremissing=True)
        j.splitter.maxFiles=-1

        j.outputfiles=[outputfile, summaryfile]

        j.backend=Dirac()
        queues.add(j.submit)
