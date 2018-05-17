def options( year, mag, data_type, mode): 

    #======================================#
    #=== DaVinci script for Data and MC ===#
    #======================================#
    #=== year = year of data taking   ===#
    #=== mag  = magnet polarity of data ===#
    #=== data_type = MC or data?    ===#
    #======================================#
    #=== These variables are given to   ===#
    #=== function by the ganga script,  ===#
    #=== which is different, depending  ===#
    #=== on whether MC or data is being ===#
    #=== looked at.           ===#
    #======================================#
    from Configurables import DaVinci
    from Configurables import MCMatchObjP2MCRelator
    from PhysConf.Filters import LoKi_Filters
    #===========================#
    #=== Check if MC or Data ===#
    #===========================#  
    if data_type  == "data":
        data = True
    elif data_type  == "MC":
        data = False
    else:
        print "WARNING: data type not recognised. Please enter either \"data\" or \"MC\" as the third argument to options()!"

    #====================================#
    #=== Setup depending on data type ===#
    #====================================#
    if data:
        stream  = 'Bhadron'  
        prefix  = 'Data'
    else:
        stream  = 'AllStreams'
        prefix  = 'MC'

    #==============================#
    #=== Assign Stripping Lines ===#
    #==============================#

    #=== b2dk d2pi0hh  ===#
    line_k  = 'B2D0KD2Pi0HHResolvedBeauty2CharmLine'
    #=== b2dpi d2pi0hh ===#
    line_pi = 'B2D0PiD2Pi0HHResolvedBeauty2CharmLine'

    #=============================#
    #=== Stripping Pre-Filters ===#
    #=============================#
    fltrs = LoKi_Filters (
        STRIP_Code = "HLT_PASS_RE('StrippingB2D0KD2Pi0HHResolvedBeauty2CharmLineDecision') | HLT_PASS_RE('StrippingB2D0PiD2Pi0HHResolvedBeauty2CharmLineDecision')"
    )

    #==============================#
    #=== Import necessary tools ===#
    #==============================#
    from Configurables import DecayTreeTuple
    from DecayTreeTuple.Configuration import *
    if data:
        from Configurables import TrackScaleState
        scaler = TrackScaleState( 'StateScale' )
        scaler.RootInTES = '/Event/{0}/'.format(stream)

    #===================================================#
    #=== Setup DecayTreeTuples for each channel/mode ===#
    #===================================================#
    etuple  = EventTuple()
    if data:
        dk_d2kpipi0    = DecayTreeTuple('B2DK_D2KPiPi0')
        dk_d2pikpi0    = DecayTreeTuple('B2DK_D2PiKPi0')
        dk_d2kkpi0    = DecayTreeTuple('B2DK_D2KKPi0')
        dk_d2pipipi0  = DecayTreeTuple('B2DK_D2PiPiPi0')
        dpi_d2kpipi0  = DecayTreeTuple('B2DPi_D2KPiPi0')
        dpi_d2pikpi0  = DecayTreeTuple('B2DPi_D2PiKPi0')
        dpi_d2kkpi0    = DecayTreeTuple('B2DPi_D2KKPi0')
        dpi_d2pipipi0  = DecayTreeTuple('B2DPi_D2PiPiPi0')
    else:
        dk_d2kpipi0    = DecayTreeTuple('B2DK_D2KPiPi0_MC')
        dk_d2pikpi0    = DecayTreeTuple('B2DK_D2PiKPi0_MC')
        dk_d2kkpi0    = DecayTreeTuple('B2DK_D2KKPi0_MC')
        dk_d2pipipi0  = DecayTreeTuple('B2DK_D2PiPiPi0_MC')
        dpi_d2kpipi0  = DecayTreeTuple('B2DPi_D2KPiPi0_MC')
        dpi_d2pikpi0  = DecayTreeTuple('B2DPi_D2PiKPi0_MC')
        dpi_d2kkpi0    = DecayTreeTuple('B2DPi_D2KKPi0_MC')
        dpi_d2pipipi0  = DecayTreeTuple('B2DPi_D2PiPiPi0_MC')

    #=== Assign vectors of DecayTreeTuples for easy use later ===#
    channels = [dk_d2kpipi0, dk_d2pikpi0, dk_d2kkpi0, dk_d2pipipi0,
                dpi_d2kpipi0, dpi_d2pikpi0, dpi_d2kkpi0, dpi_d2pipipi0]

    k_chans  = [dk_d2kpipi0, dk_d2pikpi0, dk_d2kkpi0, dk_d2pipipi0]
    pi_chans = [dpi_d2kpipi0, dpi_d2pikpi0, dpi_d2kkpi0, dpi_d2pipipi0]

    #=== Setting location/inputs is different depending on data type ===#
    if data:
        for channel in channels:
            channel.RootInTES = '/Event/{0}'.format(stream)
        for channel in k_chans:
            channel.Inputs  = ['Phys/{0}/Particles'.format(line_k)]
        for channel in pi_chans:
            channel.Inputs  = ['Phys/{0}/Particles'.format(line_pi)]

    else:
        for channel in k_chans:
            if year == '2012' or year == '2011': # 2012 and 2011 MC is DST!
                channel.Inputs  = ['/Event/{0}/Phys/{1}/Particles'.format(stream, line_k)]
            else:
                channel.Inputs  = ['Phys/{0}/Particles'.format(line_k)]
        for channel in pi_chans:
            if year == '2012' or year == '2011':
                channel.Inputs  = ['/Event/{0}/Phys/{1}/Particles'.format(stream, line_pi)]
            else:
                channel.Inputs  = ['Phys/{0}/Particles'.format(line_pi)]

    #=====================================================#
    #=== Setup decay descriptors for each channel/mode ===#
    #=====================================================#
    #=== B2DK ===#
    dk_d2kpipi0.Decay    = '[(B+ --> ^(D0 --> ^K+ ^pi- ^(pi0 --> ^gamma ^gamma)) ^K+)  ,  (B- --> ^(D0 --> ^K- ^pi+ ^(pi0 --> ^gamma ^gamma)) ^K- )]'
    dk_d2kpipi0.addBranches({  'Bu'  :  '[(B+ --> (D0 --> K+ pi- (pi0 --> gamma gamma)) K+ )  ,  (B- --> (D0 --> K- pi+ (pi0 --> gamma gamma)) K-  )]',
                             'D0'  :  '[(B+ --> ^(D0 --> K+ pi- (pi0 --> gamma gamma)) K+)  ,  (B- --> ^(D0 --> K- pi+ (pi0 --> gamma gamma)) K- )]',
                             'K0'  :  '[(B+ --> (D0 --> ^K+ pi- (pi0 --> gamma gamma)) K+)  ,  (B- --> (D0 --> ^K- pi+ (pi0 --> gamma gamma)) K- )]',
                             'P0'  :  '[(B+ --> (D0 --> K+ ^pi- (pi0 --> gamma gamma)) K+)  ,  (B- --> (D0 --> K- ^pi+ (pi0 --> gamma gamma)) K- )]',
                             'Pi0'  :  '[(B+ --> (D0 --> K+ pi- ^(pi0 --> gamma gamma)) K+)  ,  (B- --> (D0 --> K- pi+ ^(pi0 --> gamma gamma)) K- )]',
                             'Bach'  :  '[(B+ --> (D0 --> K+ pi- (pi0 --> gamma gamma)) ^K+)  ,  (B- --> (D0 --> K- pi+ (pi0 --> gamma gamma)) ^K- )]',
                             'Gamma1':  '[(B+ --> (D0 --> K+ pi- (pi0 --> ^gamma gamma)) K+)  ,  (B- --> (D0 --> K- pi+ (pi0 --> ^gamma gamma)) K- )]',
                             'Gamma2':  '[(B+ --> (D0 --> K+ pi- (pi0 --> gamma ^gamma)) K+)  ,  (B- --> (D0 --> K- pi+ (pi0 --> gamma ^gamma)) K- )]'
                            })

    dk_d2pikpi0.Decay    = '[(B+ --> ^(D0 --> ^pi+ ^K- ^(pi0 --> ^gamma ^gamma)) ^K+)  ,  (B- --> ^(D0 --> ^pi- ^K+ ^(pi0 --> ^gamma ^gamma)) ^K- )]'
    dk_d2pikpi0.addBranches({  'Bu'  :  '[(B+ --> (D0 --> pi+ K- (pi0 --> gamma gamma)) K+ )  ,  (B- --> (D0 --> pi- K+ (pi0 --> gamma gamma)) K-  )]',
                             'D0'  :  '[(B+ --> ^(D0 --> pi+ K- (pi0 --> gamma gamma)) K+)  ,  (B- --> ^(D0 --> pi- K+ (pi0 --> gamma gamma)) K- )]',
                             'P0'  :  '[(B+ --> (D0 --> ^pi+ K- (pi0 --> gamma gamma)) K+)  ,  (B- --> (D0 --> ^pi- K+ (pi0 --> gamma gamma)) K- )]',
                             'K0'  :  '[(B+ --> (D0 --> pi+ ^K- (pi0 --> gamma gamma)) K+)  ,  (B- --> (D0 --> pi- ^K+ (pi0 --> gamma gamma)) K- )]',
                             'Pi0'  :  '[(B+ --> (D0 --> pi+ K- ^(pi0 --> gamma gamma)) K+)  ,  (B- --> (D0 --> pi- K+ ^(pi0 --> gamma gamma)) K- )]',
                             'Bach'  :  '[(B+ --> (D0 --> pi+ K- (pi0 --> gamma gamma)) ^K+)  ,  (B- --> (D0 --> pi- K+ (pi0 --> gamma gamma)) ^K- )]',
                             'Gamma1':  '[(B+ --> (D0 --> pi+ K- (pi0 --> ^gamma gamma)) K+)  ,  (B- --> (D0 --> pi- K+ (pi0 --> ^gamma gamma)) K- )]',
                             'Gamma2':  '[(B+ --> (D0 --> pi+ K- (pi0 --> gamma ^gamma)) K+)  ,  (B- --> (D0 --> pi- K+ (pi0 --> gamma ^gamma)) K- )]'
                            })

    dk_d2kkpi0.Decay    = '[(B+ --> ^(D0 --> ^K+ ^K- ^(pi0 --> ^gamma ^gamma)) ^K+)  ,  (B- --> ^(D0 --> ^K- ^K+ ^(pi0 --> ^gamma ^gamma)) ^K- )]'
    dk_d2kkpi0.addBranches({  'Bu'  :  '[(B+ --> (D0 --> K+ K- (pi0 --> gamma gamma)) K+ )  ,  (B- --> (D0 --> K- K+ (pi0 --> gamma gamma)) K-  )]',
                            'D0'  :  '[(B+ --> ^(D0 --> K+ K- (pi0 --> gamma gamma)) K+)  ,  (B- --> ^(D0 --> K- K+ (pi0 --> gamma gamma)) K- )]',
                            'K0'  :  '[(B+ --> (D0 --> ^K+ K- (pi0 --> gamma gamma)) K+)  ,  (B- --> (D0 --> ^K- K+ (pi0 --> gamma gamma)) K- )]',
                            'K1'  :  '[(B+ --> (D0 --> K+ ^K- (pi0 --> gamma gamma)) K+)  ,  (B- --> (D0 --> K- ^K+ (pi0 --> gamma gamma)) K- )]',
                            'Pi0'  :  '[(B+ --> (D0 --> K+ K- ^(pi0 --> gamma gamma)) K+)  ,  (B- --> (D0 --> K- K+ ^(pi0 --> gamma gamma)) K- )]',
                            'Bach'  :  '[(B+ --> (D0 --> K+ K- (pi0 --> gamma gamma)) ^K+)  ,  (B- --> (D0 --> K- K+ (pi0 --> gamma gamma)) ^K- )]',
                            'Gamma1':  '[(B+ --> (D0 --> K+ K- (pi0 --> ^gamma gamma)) K+)  ,  (B- --> (D0 --> K- K+ (pi0 --> ^gamma gamma)) K- )]',
                            'Gamma2':  '[(B+ --> (D0 --> K+ K- (pi0 --> gamma ^gamma)) K+)  ,  (B- --> (D0 --> K- K+ (pi0 --> gamma ^gamma)) K- )]'
                           })

    dk_d2pipipi0.Decay    = '[(B+ --> ^(D0 --> ^pi+ ^pi- ^(pi0 --> ^gamma ^gamma)) ^K+)  ,  (B- --> ^(D0 --> ^pi- ^pi+ ^(pi0 --> ^gamma ^gamma)) ^K- )]'
    dk_d2pipipi0.addBranches({  'Bu'  :  '[(B+ --> (D0 --> pi+ pi- (pi0 --> gamma gamma)) K+ )  ,  (B- --> (D0 --> pi- pi+ (pi0 --> gamma gamma)) K-  )]',
                              'D0'  :  '[(B+ --> ^(D0 --> pi+ pi- (pi0 --> gamma gamma)) K+)  ,  (B- --> ^(D0 --> pi- pi+ (pi0 --> gamma gamma)) K- )]',
                              'P0'  :  '[(B+ --> (D0 --> ^pi+ pi- (pi0 --> gamma gamma)) K+)  ,  (B- --> (D0 --> ^pi- pi+ (pi0 --> gamma gamma)) K- )]',
                              'P1'  :  '[(B+ --> (D0 --> pi+ ^pi- (pi0 --> gamma gamma)) K+)  ,  (B- --> (D0 --> pi- ^pi+ (pi0 --> gamma gamma)) K- )]',
                              'Pi0'  :  '[(B+ --> (D0 --> pi+ pi- ^(pi0 --> gamma gamma)) K+)  ,  (B- --> (D0 --> pi- pi+ ^(pi0 --> gamma gamma)) K- )]',
                              'Bach'  :  '[(B+ --> (D0 --> pi+ pi- (pi0 --> gamma gamma)) ^K+)  ,  (B- --> (D0 --> pi- pi+ (pi0 --> gamma gamma)) ^K- )]',
                              'Gamma1':  '[(B+ --> (D0 --> pi+ pi- (pi0 --> ^gamma gamma)) K+)  ,  (B- --> (D0 --> pi- pi+ (pi0 --> ^gamma gamma)) K- )]',
                              'Gamma2':  '[(B+ --> (D0 --> pi+ pi- (pi0 --> gamma ^gamma)) K+)  ,  (B- --> (D0 --> pi- pi+ (pi0 --> gamma ^gamma)) K- )]'
                             })



    #=== B2DPi ===#
    dpi_d2kpipi0.Decay    = '[(B+ --> ^(D0 --> ^K+ ^pi- ^(pi0 --> ^gamma ^gamma)) ^pi+)  ,  (B- --> ^(D0 --> ^K- ^pi+ ^(pi0 --> ^gamma ^gamma)) ^pi- )]'
    dpi_d2kpipi0.addBranches({  'Bu'  :  '[(B+ --> (D0 --> K+ pi- (pi0 --> gamma gamma)) pi+ )  ,  (B- --> (D0 --> K- pi+ (pi0 --> gamma gamma)) pi-  )]',
                              'D0'  :  '[(B+ --> ^(D0 --> K+ pi- (pi0 --> gamma gamma)) pi+)  ,  (B- --> ^(D0 --> K- pi+ (pi0 --> gamma gamma)) pi- )]',
                              'K0'  :  '[(B+ --> (D0 --> ^K+ pi- (pi0 --> gamma gamma)) pi+)  ,  (B- --> (D0 --> ^K- pi+ (pi0 --> gamma gamma)) pi- )]',
                              'P0'  :  '[(B+ --> (D0 --> K+ ^pi- (pi0 --> gamma gamma)) pi+)  ,  (B- --> (D0 --> K- ^pi+ (pi0 --> gamma gamma)) pi- )]',
                              'Pi0'  :  '[(B+ --> (D0 --> K+ pi- ^(pi0 --> gamma gamma)) pi+)  ,  (B- --> (D0 --> K- pi+ ^(pi0 --> gamma gamma)) pi- )]',
                              'Bach'  :  '[(B+ --> (D0 --> K+ pi- (pi0 --> gamma gamma)) ^pi+)  ,  (B- --> (D0 --> K- pi+ (pi0 --> gamma gamma)) ^pi- )]',
                              'Gamma1':  '[(B+ --> (D0 --> K+ pi- (pi0 --> ^gamma gamma)) pi+)  ,  (B- --> (D0 --> K- pi+ (pi0 --> ^gamma gamma)) pi- )]',
                              'Gamma2':  '[(B+ --> (D0 --> K+ pi- (pi0 --> gamma ^gamma)) pi+)  ,  (B- --> (D0 --> K- pi+ (pi0 --> gamma ^gamma)) pi- )]'
                             })

    dpi_d2pikpi0.Decay    = '[(B+ --> ^(D0 --> ^pi+ ^K- ^(pi0 --> ^gamma ^gamma)) ^pi+)  ,  (B- --> ^(D0 --> ^pi- ^K+ ^(pi0 --> ^gamma ^gamma)) ^pi-)]'
    dpi_d2pikpi0.addBranches({  'Bu'  :  '[(B+ --> (D0 --> pi+ K- (pi0 --> gamma gamma)) pi+ )  ,  (B- --> (D0 --> pi- K+ (pi0 --> gamma gamma)) pi-  )]',
                              'D0'  :  '[(B+ --> ^(D0 --> pi+ K- (pi0 --> gamma gamma)) pi+)  ,  (B- --> ^(D0 --> pi- K+ (pi0 --> gamma gamma)) pi-)]',
                              'P0'  :  '[(B+ --> (D0 --> ^pi+ K- (pi0 --> gamma gamma)) pi+)  ,  (B- --> (D0 --> ^pi- K+ (pi0 --> gamma gamma)) pi-)]',
                              'K0'  :  '[(B+ --> (D0 --> pi+ ^K- (pi0 --> gamma gamma)) pi+)  ,  (B- --> (D0 --> pi- ^K+ (pi0 --> gamma gamma)) pi-)]',
                              'Pi0'  :  '[(B+ --> (D0 --> pi+ K- ^(pi0 --> gamma gamma)) pi+)  ,  (B- --> (D0 --> pi- K+ ^(pi0 --> gamma gamma)) pi-)]',
                              'Bach'  :  '[(B+ --> (D0 --> pi+ K- (pi0 --> gamma gamma)) ^pi+)  ,  (B- --> (D0 --> pi- K+ (pi0 --> gamma gamma)) ^pi-)]',
                              'Gamma1':  '[(B+ --> (D0 --> pi+ K- (pi0 --> ^gamma gamma)) pi+)  ,  (B- --> (D0 --> pi- K+ (pi0 --> ^gamma gamma)) pi-)]',
                              'Gamma2':  '[(B+ --> (D0 --> pi+ K- (pi0 --> gamma ^gamma)) pi+)  ,  (B- --> (D0 --> pi- K+ (pi0 --> gamma ^gamma)) pi-)]'
                             })

    dpi_d2kkpi0.Decay    = '[(B+ --> ^(D0 --> ^K+ ^K- ^(pi0 --> ^gamma ^gamma)) ^pi+)  ,  (B- --> ^(D0 --> ^K- ^K+ ^(pi0 --> ^gamma ^gamma)) ^pi-)]'
    dpi_d2kkpi0.addBranches({  'Bu'  :  '[(B+ --> (D0 --> K+ K- (pi0 --> gamma gamma)) pi+ )  ,  (B- --> (D0 --> K- K+ (pi0 --> gamma gamma)) pi-  )]',
                             'D0'  :  '[(B+ --> ^(D0 --> K+ K- (pi0 --> gamma gamma)) pi+)  ,  (B- --> ^(D0 --> K- K+ (pi0 --> gamma gamma)) pi-)]',
                             'K0'  :  '[(B+ --> (D0 --> ^K+ K- (pi0 --> gamma gamma)) pi+)  ,  (B- --> (D0 --> ^K- K+ (pi0 --> gamma gamma)) pi-)]',
                             'K1'  :  '[(B+ --> (D0 --> K+ ^K- (pi0 --> gamma gamma)) pi+)  ,  (B- --> (D0 --> K- ^K+ (pi0 --> gamma gamma)) pi-)]',
                             'Pi0'  :  '[(B+ --> (D0 --> K+ K- ^(pi0 --> gamma gamma)) pi+)  ,  (B- --> (D0 --> K- K+ ^(pi0 --> gamma gamma)) pi-)]',
                             'Bach'  :  '[(B+ --> (D0 --> K+ K- (pi0 --> gamma gamma)) ^pi+)  ,  (B- --> (D0 --> K- K+ (pi0 --> gamma gamma)) ^pi-)]',
                             'Gamma1':  '[(B+ --> (D0 --> K+ K- (pi0 --> ^gamma gamma)) pi+)  ,  (B- --> (D0 --> K- K+ (pi0 --> ^gamma gamma)) pi-)]',
                             'Gamma2':  '[(B+ --> (D0 --> K+ K- (pi0 --> gamma ^gamma)) pi+)  ,  (B- --> (D0 --> K- K+ (pi0 --> gamma ^gamma)) pi-)]'
                            })

    dpi_d2pipipi0.Decay    = '[(B+ --> ^(D0 --> ^pi+ ^pi- ^(pi0 --> ^gamma ^gamma)) ^pi+)  ,  (B- --> ^(D0 --> ^pi- ^pi+ ^(pi0 --> ^gamma ^gamma)) ^pi- )]'
    dpi_d2pipipi0.addBranches({  'Bu'  :  '[(B+ --> (D0 --> pi+ pi- (pi0 --> gamma gamma)) pi+ )  ,  (B- --> (D0 --> pi- pi+ (pi0 --> gamma gamma)) pi-  )]',
                               'D0'  :  '[(B+ --> ^(D0 --> pi+ pi- (pi0 --> gamma gamma)) pi+)  ,  (B- --> ^(D0 --> pi- pi+ (pi0 --> gamma gamma)) pi- )]',
                               'P0'  :  '[(B+ --> (D0 --> ^pi+ pi- (pi0 --> gamma gamma)) pi+)  ,  (B- --> (D0 --> ^pi- pi+ (pi0 --> gamma gamma)) pi- )]',
                               'P1'  :  '[(B+ --> (D0 --> pi+ ^pi- (pi0 --> gamma gamma)) pi+)  ,  (B- --> (D0 --> pi- ^pi+ (pi0 --> gamma gamma)) pi- )]',
                               'Pi0'  :  '[(B+ --> (D0 --> pi+ pi- ^(pi0 --> gamma gamma)) pi+)  ,  (B- --> (D0 --> pi- pi+ ^(pi0 --> gamma gamma)) pi- )]',
                               'Bach'  :  '[(B+ --> (D0 --> pi+ pi- (pi0 --> gamma gamma)) ^pi+)  ,  (B- --> (D0 --> pi- pi+ (pi0 --> gamma gamma)) ^pi- )]',
                               'Gamma1':  '[(B+ --> (D0 --> pi+ pi- (pi0 --> ^gamma gamma)) pi+)  ,  (B- --> (D0 --> pi- pi+ (pi0 --> ^gamma gamma)) pi- )]',
                               'Gamma2':  '[(B+ --> (D0 --> pi+ pi- (pi0 --> gamma ^gamma)) pi+)  ,  (B- --> (D0 --> pi- pi+ (pi0 --> gamma ^gamma)) pi- )]'
                              })

    #========================================#
    #=== Setup LoKi variables to be added ===#
    #========================================#
    #=== Added to all branches ===#
    LoKiVars = {
        "Q"        : "Q",
        "DIRA_BPV"    : "BPVDIRA",
        "MAXDOCA"     : "DOCAMAX",
        "AMAXDOCA"    : "PFUNA(AMAXDOCA(''))",
        "MIPCHI2_PV"  : "MIPCHI2DV(PRIMARY)",
        "VTXCHI2DOF"  : "VFASPF(VCHI2/VDOF)",
        "LT_BPV"    : "BPVLTIME('PropertimeFitter/ProperTime::PUBLIC')",
    }

    if data:
        #=== Added only to B2DK channels ===#
        LoKiVars_K = {
            "ptasy_1.50"  : "RELINFO('/Event/Bhadron/Phys/B2D0KD2Pi0HHResolvedBeauty2CharmLine/P2ConeVar1','CONEPTASYM',-1000.)"
        }

        #=== Added only to B2DPi channels ===#
        LoKiVars_Pi= {
            "ptasy_1.50"  : "RELINFO('/Event/Bhadron/Phys/B2D0PiD2Pi0HHResolvedBeauty2CharmLine/P2ConeVar1','CONEPTASYM',-1000.)"
        }

    #============================================#
    #=== Add tuple tools to relevent branches ===#
    #============================================#

    #=== Needed for new MC which has "turbo" in LFN path name ===#
    default_rel_locs = MCMatchObjP2MCRelator().getDefaultProperty('RelTableLocations')
    rel_locs = [loc for loc in default_rel_locs if 'Turbo' not in loc]

    if not data:
        etuple.addTupleTool("TupleToolGeneration")
        etuple.addTupleTool("TupleToolTrigger")

    for channel in channels:
        #=== Added to all branches ===#
        channel.ToolList += ["TupleToolAngles"     ,
                             "TupleToolEventInfo"  ,
                             "TupleToolGeometry"   ,
                             "TupleToolKinematic"  ,
                             "TupleToolPid"      ,
                             "TupleToolPrimaries"  ,
                             "TupleToolPropertime"   ,
                             "TupleToolRecoStats"  ,
                             "TupleToolTrackInfo"  
                            ]

        if not data:
            ttMCt = channel.addTupleTool("TupleToolMCTruth")
            ttMCt.addTool(MCMatchObjP2MCRelator)
            ttMCt.MCMatchObjP2MCRelator.RelTableLocations = rel_locs

        channel.addTupleTool("LoKi::Hybrid::TupleTool/LoKi_All")
        channel.LoKi_All.Variables = LoKiVars

        #=== Neutral variables added to photons from pi0 decay ===#
        channel.Gamma1.addTupleTool("TupleToolPhotonInfo")
        channel.Gamma1.addTupleTool("TupleToolCaloHypo")
        channel.Gamma1.addTupleTool("TupleToolProtoPData/ProtoPData")
        channel.Gamma1.ProtoPData.DataList = ["IsNotE"]

        channel.Gamma2.addTupleTool("TupleToolPhotonInfo")
        channel.Gamma2.addTupleTool("TupleToolCaloHypo")
        channel.Gamma2.addTupleTool("TupleToolProtoPData/ProtoPData")
        channel.Gamma2.ProtoPData.DataList = ["IsNotE"]

        #=== Add Pi0 information ===#
        channel.Pi0.addTupleTool("TupleToolPi0Info")
        channel.Pi0.addTupleTool("TupleToolCaloHypo")

        #=== Trigger information added to Bu ===#
        channel.Bu.addTupleTool("TupleToolTISTOS")
        channel.Bu.TupleToolTISTOS.Verbose = True
        # Trigger names changed with runs!
        TriggerListL0 = ["L0HadronDecision"]
        TriggerListHlt1 = []
        TriggerListHlt2 = []

        if year == '2011' or year == '2012':
            TriggerListHlt1 = ["Hlt1TrackAllL0Decision"]
            TriggerListHlt2 = ["Hlt2Topo2BodyBBDTDecision", "Hlt2Topo3BodyBBDTDecision", "Hlt2Topo4BodyBBDTDecision"]

        if year == '2015' or year == '2016' or year == '2017':
            TriggerListHlt1 = ["Hlt1TrackMVADecision", "Hlt1TwoTrackMVADecision"]
            TriggerListHlt2 = ["Hlt2Topo2BodyDecision", "Hlt2Topo3BodyDecision", "Hlt2Topo4BodyDecision"]

        AllTriggers = TriggerListL0 + TriggerListHlt1 + TriggerListHlt2
        channel.Bu.TupleToolTISTOS.TriggerList = AllTriggers

        #=== Kinematic refit of Bu based on fixing D0 mass and fixing origin vertex ===#
        #=== DPVCFIT = D0 (mass), primary vertex constrained fit ===# 
        channel.Bu.addTupleTool("TupleToolDecayTreeFitter/DPVCFIT")
        channel.Bu.DPVCFIT.constrainToOriginVertex = True
        channel.Bu.DPVCFIT.Verbose = True
        channel.Bu.DPVCFIT.daughtersToConstrain = ["D0"]
        channel.Bu.DPVCFIT.UpdateDaughters = True


        #=== Add branches which are exclusive to MC data ===#
        if not data:
            channel.ToolList += [ "TupleToolMCBackgroundInfo" ]
            ttMCt.ToolList = [
                "MCTupleToolDecayType", 
                "MCTupleToolHierarchy",
                "MCTupleToolKinematic",
                "MCTupleToolReconstructed"
            ]

            #=== Add ptasy_1.50 variable ===#
            channel.addTupleTool("TupleToolTrackIsolation/ttti")
            channel.ttti.FillAsymmetry = True
            channel.ttti.MinConeAngle = 1.5
            channel.ttti.MaxConeAngle = 1.5 

    #=== Add ptasy_1.50 variable for data ===#
    if data:
        for channel in k_chans:
            channel.Bu.addTupleTool("LoKi::Hybrid::TupleTool/LoKi_Bu")
            channel.Bu.LoKi_Bu.Variables = dict(LoKiVars.items() + LoKiVars_K.items())
            channel.Pi0.addTupleTool("LoKi::Hybrid::TupleTool/LoKi_Pi0")
            channel.Pi0.LoKi_Pi0.Variables = dict(LoKiVars.items() + LoKiVars_K.items())
            channel.Gamma1.addTupleTool("LoKi::Hybrid::TupleTool/LoKi_Gamma1")
            channel.Gamma1.LoKi_Gamma1.Variables = dict(LoKiVars.items() + LoKiVars_K.items())
            channel.Gamma2.addTupleTool("LoKi::Hybrid::TupleTool/LoKi_Gamma2")
            channel.Gamma2.LoKi_Gamma2.Variables = dict(LoKiVars.items() + LoKiVars_K.items())

        for channel in pi_chans:
            channel.Bu.addTupleTool("LoKi::Hybrid::TupleTool/LoKi_Bu")
            channel.Bu.LoKi_Bu.Variables = dict(LoKiVars.items() + LoKiVars_Pi.items())
            channel.Pi0.addTupleTool("LoKi::Hybrid::TupleTool/LoKi_Pi0")
            channel.Pi0.LoKi_Pi0.Variables = dict(LoKiVars.items() + LoKiVars_Pi.items())
            channel.Gamma1.addTupleTool("LoKi::Hybrid::TupleTool/LoKi_Gamma1")
            channel.Gamma1.LoKi_Gamma1.Variables = dict(LoKiVars.items() + LoKiVars_Pi.items())
            channel.Gamma2.addTupleTool("LoKi::Hybrid::TupleTool/LoKi_Gamma2")
            channel.Gamma2.LoKi_Gamma2.Variables = dict(LoKiVars.items() + LoKiVars_Pi.items())

    #==============================#
    #=== Finalise configuration ===#
    #==============================#
    #=== Set database tags ===#
    from Configurables import CondDB
    if data:
        CondDB( LatestGlobalTagByDataType = year )

    #=== Configure DaVinci ===#

    if data:
        DaVinci().RootInTES = '/Event/{0}'.format(stream)
        DaVinci().appendToMainSequence( [scaler] )
        DaVinci().EventPreFilters = fltrs.filters('Filters')
        channels += [etuple]
        DaVinci().UserAlgorithms += channels

    #=== Input type for 2012 MC is DST, but all data is MDST ===#
    if data:
        DaVinci().InputType = 'MDST'
        DaVinci().TupleFile = prefix+'_Bu2DH_AllModes_'+year+'_Mag'+mag+'.root'
    if not data:
        DaVinci().TupleFile = prefix+'_Bu2'+mode+'_'+year+'_Mag'+mag+'.root'
        if year == "2012" or year == "2011":
            DaVinci().InputType = 'DST'
        else:
            DaVinci().InputType = 'MDST'
            DaVinci().RootInTES = '/Event/{0}'.format(stream)

    DaVinci().PrintFreq = 1000
    DaVinci().DataType = year
    DaVinci().Simulation = not data
    DaVinci().Lumi = not DaVinci().Simulation
    DaVinci().EvtMax = -1
    DaVinci().RootCompressionLevel = 'ZLIB:1'

    #=======================================================#
    #=== Set database tags! Only needs to be done for MC ===#
    #=======================================================#
    if not data:
        #=== D --> K Pi Pi0 ===#
        if mode == "DK_D2KPiPi0" or mode == "DPi_D2KPiPi0":
            if year == "2012":
                if mag == 'Up':
                    DaVinci().CondDBtag = 'sim-20160321-2-vc-mu100'
                if mag == 'Down':
                    DaVinci().CondDBtag = 'sim-20160321-2-vc-md100'
                    DaVinci().DDDBtag = 'dddb-20150928'
            if year == "2015":
                if mag == 'Up':
                    DaVinci().CondDBtag = 'sim-20161124-vc-mu100'
                if mag == 'Down':
                    DaVinci().CondDBtag = 'sim-20161124-vc-md100'
            DaVinci().DDDBtag = 'dddb-20170721-3'

        #=== D --> Pi Pi Pi0 ===#
        if mode == "DK_D2PiPiPi0" or mode == "DPi_D2PiPiPi0":
            if year == "2012":
                if mag == 'Up':
                    DaVinci().CondDBtag = 'sim-20130522-1-vc-mu100'
                if mag == 'Down':
                    DaVinci().CondDBtag = 'sim-20130522-1-vc-md100'
            DaVinci().DDDBtag = 'dddb-20130929-1'
        #=== D --> K K Pi0 ===#
        if mode == "DK_D2KKPi0"  or mode == "DPi_D2KKPi0":
            if year == "2012":
                if mag == 'Up':
                    DaVinci().CondDBtag = 'sim-20130522-1-vc-mu100'
                if mag == 'Down':
                    DaVinci().CondDBtag = 'sim-20130522-1-vc-md100'
            DaVinci().DDDBtag = 'dddb-20130929-1'


    return

    #============================================================#
    #=== LOCAL TESTING CODE (Make sure indentation is right!) ===#
    #============================================================#
    #=== This code is used if you want to run on local files  ===#
    #=== instead of on the grid                 ===#
    #============================================================#


    #from GaudiConf import IOHelper

    # Use the local input data

    #ioh =  IOHelper()
    #ioh.inputFiles([
        #'/home/colmmurphy/00069695_00000002_5.AllStreams.dst', # 2011, B2DPi_D2KPiPi0, Down, MC
        #'/home/colmmurphy/00041840_00000015_1.bhadron.mdst' ,# 2011, Down, Data
        #'/home/colmmurphy/00069527_00000010_1.bhadron.mdst' ,# 2016, Down, Data

    #], clear=True)


    #return
#options("2011", "Down", "data", "DPi_D2KPiPi0")

