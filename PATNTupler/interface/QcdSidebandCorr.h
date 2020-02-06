#ifndef QCDSIDEBANDCORR_H
#define QCDSIDEBANDCORR_H

// a method to get the QCD sideband correction factor
// corr_{i} = S_{i}^{anti} / U+D_{i}^{anti}

namespace QcdSidebandCorr{

	double GetCorr(const unsigned int& binIndex, const unsigned int& yearOfRun){

		// #*,. #*,. #*,. #*,.
		// #*,. 2016 #*,. #*,.
		// #*,. #*,. #*,. #*,.
		if (yearOfRun == 2016){

			std::vector<double> corrVec;
			
			// ht1500to2500 - normal method
			// corrVec.push_back(1.6064209275);
			// corrVec.push_back(0.7789782091);
			// corrVec.push_back(0.8267942584);
			// corrVec.push_back(0.9253012048);
			// corrVec.push_back(0.9914814815);
			// corrVec.push_back(1.1304957905);
			// corrVec.push_back(1.1731970471);
			// corrVec.push_back(1.1436619718);
			// corrVec.push_back(1.0693493151);
			// corrVec.push_back(1.1156626506);

			// ht1500to2500 - calculation (data)
			corrVec.push_back(1.55293913318);
			corrVec.push_back(0.78491606676);
			corrVec.push_back(0.885498640061);
			corrVec.push_back(0.946245611227);
			corrVec.push_back(0.990664428178);
			corrVec.push_back(1.0305087463);
			corrVec.push_back(1.0719120106);
			corrVec.push_back(1.11813646669);
			corrVec.push_back(1.16872864);
			corrVec.push_back(1.21319670302);

			// ht2500to3500 - calculation (data)
			corrVec.push_back(1.41807821395);
			corrVec.push_back(0.661581716844);
			corrVec.push_back(0.783590911042);
			corrVec.push_back(0.871323261773);
			corrVec.push_back(0.931618022073);
			corrVec.push_back(0.975564306489);
			corrVec.push_back(1.00977765424);
			corrVec.push_back(1.03656901524);
			corrVec.push_back(1.05396133163);
			corrVec.push_back(1.05459175319);

			// ht3500toInf - calculation (mc)
			corrVec.push_back(1.52832414925);
			corrVec.push_back(0.633843685648);
			corrVec.push_back(0.711003843484);
			corrVec.push_back(0.795489036673);
			corrVec.push_back(0.865761298856);
			corrVec.push_back(0.921327681259);
			corrVec.push_back(0.964265274597);
			corrVec.push_back(0.995196004563);
			corrVec.push_back(1.0114604017);
			corrVec.push_back(1.0059811867);

			if (binIndex > 0 && binIndex < corrVec.size() + 1 ) return corrVec[binIndex - 1];
			else{
				std::cout << "WARNING: you haven't provided a valid index for the QCD 2016 sideband correction factor" << std::endl;
				std::cout << "You are being given a dummy value of -123456789.0" << std::endl;
				return -123456789.0;
			}
		} // closes 'if' yearOfRun==2016


		// #*,. #*,. #*,. #*,.
		// #*,. 2017 #*,. #*,.
		// #*,. #*,. #*,. #*,.
		else if (yearOfRun == 2017){
			
			std::vector<double> corrVec;

			// ht1500to2500 - normal method
			// corrVec.push_back(1.7026466009);
			// corrVec.push_back(0.7909370200);
			// corrVec.push_back(0.8413654618);
			// corrVec.push_back(0.8451202263);
			// corrVec.push_back(1.0144796380);
			// corrVec.push_back(1.0905271199);
			// corrVec.push_back(1.1422745809);
			// corrVec.push_back(1.1844380403);
			// corrVec.push_back(1.0975778547);
			// corrVec.push_back(1.0845724907);

			// ht1500to2500 - calculation (data)
			corrVec.push_back(1.57341219788);
			corrVec.push_back(0.80184834314);
			corrVec.push_back(0.897431373921);
			corrVec.push_back(0.953956438434);
			corrVec.push_back(0.995976449777);
			corrVec.push_back(1.03481842038);
			corrVec.push_back(1.07654104956);
			corrVec.push_back(1.12496368094);
			corrVec.push_back(1.18125975218);
			corrVec.push_back(1.2383590285);

			// ht2500to3500 - calculation (data)
			corrVec.push_back(1.42918156306);
			corrVec.push_back(0.679755341741);
			corrVec.push_back(0.802359530496);
			corrVec.push_back(0.887182179816);
			corrVec.push_back(0.945227120672);
			corrVec.push_back(0.987763215906);
			corrVec.push_back(1.02043537474);
			corrVec.push_back(1.0436803335);
			corrVec.push_back(1.05218236907);
			corrVec.push_back(1.03353107535);

			// ht3500toInf - calculation (mc)
			corrVec.push_back(1.52121148578);
			corrVec.push_back(0.614623424388);
			corrVec.push_back(0.685781722398);
			corrVec.push_back(0.771740450118);
			corrVec.push_back(0.849108401454);
			corrVec.push_back(0.914688904111);
			corrVec.push_back(0.969800933375);
			corrVec.push_back(1.01619503533);
			corrVec.push_back(1.05362809173);
			corrVec.push_back(1.07743843374);

			if (binIndex > 0 && binIndex < corrVec.size() + 1 ) return corrVec[binIndex - 1];
			else{
				std::cout << "WARNING: you haven't provided a valid index for the QCD 2017 sideband correction factor" << std::endl;
				std::cout << "You are being given a dummy value of -123456789.0" << std::endl;
				return -123456789.0;
			}
		} // closes 'if' yearOfRun==2017




		// #*,. #*,. #*,. #*,.
		// #*,. 2018 #*,. #*,.
		// #*,. #*,. #*,. #*,.
		else if (yearOfRun == 2018){

			std::vector<double> corrVec;

			// ht1500to2500 - normal method // old numbers
			// corrVec.push_back(1.6616815612);
			// corrVec.push_back(0.7705114255);
			// corrVec.push_back(0.8005140654);
			// corrVec.push_back(0.8899382492);
			// corrVec.push_back(1.0556079171);
			// corrVec.push_back(1.1144646925);
			// corrVec.push_back(1.1893203883);
			// corrVec.push_back(1.1813493769);
			// corrVec.push_back(1.1496488385);
			// corrVec.push_back(1.0994397759);

			// ht1500to2500 - calculation (data)
			corrVec.push_back(1.53495664724);
			corrVec.push_back(0.767780874);
			corrVec.push_back(0.873208676934);
			corrVec.push_back(0.9400173019);
			corrVec.push_back(0.988626023141);
			corrVec.push_back(1.03156834113);
			corrVec.push_back(1.07601880112);
			corrVec.push_back(1.12650434353);
			corrVec.push_back(1.18463262656);
			corrVec.push_back(1.24346002494);

			// ht2500to3500 - calculation (data)
			corrVec.push_back(1.4384092221);
			corrVec.push_back(0.675903235154);
			corrVec.push_back(0.795128249044);
			corrVec.push_back(0.880664486917);
			corrVec.push_back(0.940808471888);
			corrVec.push_back(0.986044340116);
			corrVec.push_back(1.02201595243);
			corrVec.push_back(1.04938927876);
			corrVec.push_back(1.06283519632);
			corrVec.push_back(1.0488302415);

			// ht3500toInf - calculation (mc, wide bins)
			// corrVec.push_back(1.69162900558);
			// corrVec.push_back(0.65339635843);
			// corrVec.push_back(0.668094189916);
			// corrVec.push_back(0.730958163827);
			// corrVec.push_back(0.811876492716);
			// corrVec.push_back(0.897194487237);
			// corrVec.push_back(0.978906155434);
			// corrVec.push_back(1.05194233704);
			// corrVec.push_back(1.10863586286);
			// corrVec.push_back(1.1296495168);

			// ht3500toInf - calculation (data, normal bins)
			corrVec.push_back(1.54338252847);
			corrVec.push_back(0.638023080523);
			corrVec.push_back(0.711137631263);
			corrVec.push_back(0.792151570886);
			corrVec.push_back(0.861705324027);
			corrVec.push_back(0.918924397065);
			corrVec.push_back(0.965100262833);
			corrVec.push_back(0.999898919691);
			corrVec.push_back(1.01900984127);
			corrVec.push_back(1.01216641175);

			if (binIndex > 0 && binIndex < corrVec.size() + 1 ) return corrVec[binIndex - 1];
			else{
				std::cout << "WARNING: you haven't provided a valid index for the QCD 2018 sideband correction factor" << std::endl;
				std::cout << "You are being given a dummy value of -123456789.0" << std::endl;
				return -123456789.0;
			}
		} // closes 'if' yearOfRun==2018

		else{
			std::cout << "You have not provided QcdSidebandCorr::GetCorr with a valid yearOfRun, must be 2016, 2017 or 2018" << std::endl;
			std::cout << "You are being given a dummy value of -2635426354.0" << std::endl;
			return -2635426354.0;
		}



	} // closes function GetCorr




	double GetCorrErr(const unsigned int& binIndex, const unsigned int& yearOfRun){

		// #*,. #*,. #*,. #*,.
		// #*,. 2016 #*,. #*,.
		// #*,. #*,. #*,. #*,.
		if (yearOfRun == 2016){

			std::vector<double> corrErrVec;

			// ht1500to2500 - normal method
			// corrErrVec.push_back(0.0352796250);
			// corrErrVec.push_back(0.0159298571);
			// corrErrVec.push_back(0.0190088308);
			// corrErrVec.push_back(0.0231644431);
			// corrErrVec.push_back(0.0270426071);
			// corrErrVec.push_back(0.0335637876);
			// corrErrVec.push_back(0.0380500609);
			// corrErrVec.push_back(0.0415511282);
			// corrErrVec.push_back(0.0435266537);
			// corrErrVec.push_back(0.0533274246);

			// ht1500to2500 - calculation (data)
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);

			// ht2500to3500 - calculation (data)
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);

			// ht3500toInf - calculation (mc)
			corrErrVec.push_back(0.30);
			corrErrVec.push_back(0.30);
			corrErrVec.push_back(0.30);
			corrErrVec.push_back(0.30);
			corrErrVec.push_back(0.30);
			corrErrVec.push_back(0.30);
			corrErrVec.push_back(0.30);
			corrErrVec.push_back(0.30);
			corrErrVec.push_back(0.30);
			corrErrVec.push_back(0.30);

			if (binIndex > 0 && binIndex < corrErrVec.size() + 1 ) return corrErrVec[binIndex - 1];
			else{
				std::cout << "WARNING: you haven't provided a valid index for the QCD 2016 sideband correction factor error" << std::endl;
				std::cout << "You are being given a dummy value of -123456789.0" << std::endl;
				return -123456789.0;
			}
		} // closes 'if' yearOfRun==2016


		// #*,. #*,. #*,. #*,.
		// #*,. 2017 #*,. #*,.
		// #*,. #*,. #*,. #*,.
		if (yearOfRun == 2017){

			std::vector<double> corrErrVec;

			// ht1500to2500 - normal method
			// corrErrVec.push_back(0.0345542152);
			// corrErrVec.push_back(0.0147509712);
			// corrErrVec.push_back(0.0176379321);
			// corrErrVec.push_back(0.0191728313);
			// corrErrVec.push_back(0.0248291208);
			// corrErrVec.push_back(0.0295094637);
			// corrErrVec.push_back(0.0332982690);
			// corrErrVec.push_back(0.0386168015);
			// corrErrVec.push_back(0.0399156117);
			// corrErrVec.push_back(0.0458386330);

			// ht1500to2500 - calculation (data)
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);

			// ht2500to3500 - calculation (data)
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);

			// ht3500toInf - calculation (mc)
			corrErrVec.push_back(0.30);
			corrErrVec.push_back(0.30);
			corrErrVec.push_back(0.30);
			corrErrVec.push_back(0.30);
			corrErrVec.push_back(0.30);
			corrErrVec.push_back(0.30);
			corrErrVec.push_back(0.30);
			corrErrVec.push_back(0.30);
			corrErrVec.push_back(0.30);
			corrErrVec.push_back(0.30);

			if (binIndex > 0 && binIndex < corrErrVec.size() + 1 ) return corrErrVec[binIndex - 1];
			else{
				std::cout << "WARNING: you haven't provided a valid index for the QCD 2017 sideband correction factor error" << std::endl;
				std::cout << "You are being given a dummy value of -123456789.0" << std::endl;
				return -123456789.0;
			}
		} // closes 'if' yearOfRun==2017


		// #*,. #*,. #*,. #*,.
		// #*,. 2018 #*,. #*,.
		// #*,. #*,. #*,. #*,.
		if (yearOfRun == 2018){

			std::vector<double> corrErrVec;

			// ht1500to2500 - normal method
			// corrErrVec.push_back(0.0284016012);
			// corrErrVec.push_back(0.0121837549);
			// corrErrVec.push_back(0.0143463255);
			// corrErrVec.push_back(0.0174777547);
			// corrErrVec.push_back(0.0226117290);
			// corrErrVec.push_back(0.0259033612);
			// corrErrVec.push_back(0.0300473700);
			// corrErrVec.push_back(0.0332777229);
			// corrErrVec.push_back(0.0365395465);
			// corrErrVec.push_back(0.0402043763);

			// ht1500to2500 - calculation (data)
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);

			// ht2500to3500 - calculation (data)
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);

			// ht3500toInf - calculation (mc)
			corrErrVec.push_back(0.30);
			corrErrVec.push_back(0.30);
			corrErrVec.push_back(0.30);
			corrErrVec.push_back(0.30);
			corrErrVec.push_back(0.30);
			corrErrVec.push_back(0.30);
			corrErrVec.push_back(0.30);
			corrErrVec.push_back(0.30);
			corrErrVec.push_back(0.30);
			corrErrVec.push_back(0.30);

			if (binIndex > 0 && binIndex < corrErrVec.size() + 1 ) return corrErrVec[binIndex - 1];
			else{
				std::cout << "WARNING: you haven't provided a valid index for the QCD 2018 sideband correction factor error" << std::endl;
				std::cout << "You are being given a dummy value of -123456789.0" << std::endl;
				return -123456789.0;
			}
		} // closes 'if' yearOfRun==2018


		else{
			std::cout << "You have not provided QcdSidebandCorr::GetCorrErr with a valid yearOfRun, must be 2016, 2017 or 2018" << std::endl;
			std::cout << "You are being given a dummy value of -2635426354.0" << std::endl;
			return -2635426354.0;
		}

	} // closes function GetCorrErr


} // closes namespace 'QcdSidebandCorr'
#endif