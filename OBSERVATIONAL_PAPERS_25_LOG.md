# 25 Observational Astrophysics & Solar System Dynamics Paper Replication Campaign

This catalog logs the 25 original research papers authored to analyze real Solar System and exoplanetary observational data across multiple domains using first-principles C++ and Python models.

## Master Paper Index

| Paper # | Domain / Subject | Observational Dataset | Model Engine | Bazel Target | $R^2$ / $\chi^2$ | Status |
|---|---|---|---|---|---|---|
| #1 | Saturn Ring Resonances | Cassini RSS \& VIMS Occultations | `SaturnRingResonanceAnalysisModel` | `//:saturn_ring_resonances_paper` | $0.9998$ | ✅ Completed |
| #2 | Enceladus Tidal Ocean & Ice Shell | Cassini CIRS & CDA Heat Flux | `EnceladusOceanModel` | `//:enceladus_tidal_ocean_paper` | -- | ⏳ Scheduled |
| #3 | Io Tidal Heating & Laplace Resonance | Galileo NIMS & Juno JIRAM | `IoLaplaceTidalModel` | `//:io_laplace_tidal_paper` | -- | ⏳ Scheduled |
| #4 | Jupiter Juno Gravity Harmonics | Juno GS Radio Science | `JupiterJunoGravityModel` | `//:jupiter_juno_gravity_paper` | -- | ⏳ Scheduled |
| #5 | Saturn Cassini Gravity Harmonics | Cassini Grand Finale Gravity | `SaturnCassiniGravityModel` | `//:saturn_cassini_gravity_paper` | -- | ⏳ Scheduled |
| #6 | Mercury GR Pericenter Precession | MESSENGER Radio Science | `MercuryRelativisticPrecessionModel` | `//:mercury_gr_precession_paper` | -- | ⏳ Scheduled |
| #7 | Bennu Yarkovsky Effect & Astrometry | OSIRIS-REx & Arecibo Radar | `BennuYarkovskyModel` | `//:bennu_yarkovsky_paper` | -- | ⏳ Scheduled |
| #8 | Ryugu Yarkovsky Effect | Hayabusa2 & Optical Astrometry | `RyuguYarkovskyModel` | `//:ryugu_yarkovsky_paper` | -- | ⏳ Scheduled |
| #9 | Comet 67P Non-Gravitational Acceleration | Rosetta OSIRIS & RSI Tracking | `Comet67POutgassingModel` | `//:comet67p_outgassing_paper` | -- | ⏳ Scheduled |
| #10 | Planet Nine KBO Clustering | Minor Planet Center E-TNO Data | `PlanetNineSecularModel` | `//:planet_nine_kbo_paper` | -- | ⏳ Scheduled |
| #11 | Pluto-Charon Mutual Orbit & Density | New Horizons LORRI & HST | `PlutoCharonMutualModel` | `//:pluto_charon_mutual_paper` | -- | ⏳ Scheduled |
| #12 | Eris-Dysnomia Mutual Orbit & Mass | ALMA & HST Astrometry | `ErisDysnomiaModel` | `//:eris_dysnomia_paper` | -- | ⏳ Scheduled |
| #13 | Haumea Triaxial Ellipsoid & Ring | Occultation & HST Astrometry | `HaumeaEllipsoidRingModel` | `//:haumea_ellipsoid_ring_paper` | -- | ⏳ Scheduled |
| #14 | HD 209458b Hydrodynamic Escape | HST STIS Ly-$\alpha$ & H$\alpha$ | `HD209458bPhotoevaporationModel` | `//:hd209458b_photoevaporation_paper` | -- | ⏳ Scheduled |
| #15 | HD 189733b XUV Mass Loss | HST & XMM-Newton XUV Flux | `HD189733bMassLossModel` | `//:hd189733b_mass_loss_paper` | -- | ⏳ Scheduled |
| #16 | GJ 436b Extended Ly-$\alpha$ Cloud | HST WFC3 Transit Astrometry | `GJ436bHydrogenCloudModel` | `//:gj436b_hydrogen_cloud_paper` | -- | ⏳ Scheduled |
| #17 | WASP-12b Tidal Orbital Decay | TTV & High-Precision Photometry | `WASP12bTidalDecayModel` | `//:wasp12b_tidal_decay_paper` | -- | ⏳ Scheduled |
| #18 | WASP-43b Tidal Circularization | TTV & RV Orbital Ephemeris | `WASP43bTidalCircularizationModel` | `//:wasp43b_tidal_circularization_paper` | -- | ⏳ Scheduled |
| #19 | TRAPPIST-1 TTV Resonant Chain | Spitzer & Kepler/K2 TTV | `TRAPPIST1ResonantChainModel` | `//:trappist1_resonant_chain_paper` | -- | ⏳ Scheduled |
| #20 | Kepler-223 8:6:4:3 Resonant Chain | Kepler Photometric TTV | `Kepler223ResonantChainModel` | `//:kepler223_resonant_chain_paper` | -- | ⏳ Scheduled |
| #21 | KELT-9b Ultra-Hot Thermosphere | CARMENES & HARPS-N H$\alpha$ | `KELT9bUltraHotThermosphereModel` | `//:kelt9b_thermosphere_paper` | -- | ⏳ Scheduled |
| #22 | HAT-P-11b He I 10830Å Escape | HST & Keck HIRES Spectroscopy | `HATP11bHeliumEscapeModel` | `//:hatp11b_helium_escape_paper` | -- | ⏳ Scheduled |
| #23 | TOI-560b Young Sub-Neptune Escape | JWST NIRSpec & Keck HIRES | `TOI560bSubNeptuneEscapeModel` | `//:toi560b_sub_neptune_paper` | -- | ⏳ Scheduled |
| #24 | WASP-121b Deformability & RLOF | HST & JWST Phase Curves | `WASP121bDeformabilityRLOFModel` | `//:wasp121b_deformability_paper` | -- | ⏳ Scheduled |
| #25 | LTT 9779b Ultra-Hot Neptune RLOF | TESS & CHEOPS Photometry | `LTT9779bUltraHotNeptuneModel` | `//:ltt9779b_ultra_hot_paper` | -- | ⏳ Scheduled |
