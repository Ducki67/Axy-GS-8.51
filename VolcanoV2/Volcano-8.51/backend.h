#pragma once

#define CURL_STATICLIB
#include <curl/curl.h>
#include <string>
#include "framework.h"

enum EReqType
{
	EReqType_POST,
	EReqType_GET,
	EReqType_DELETE
};

static size_t Write_Callback(char* contents, size_t size, size_t nmemb, void* RES)
{
	((std::string*)RES)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

class API
{
protected:
	CURL* curl = nullptr;
public:
	API()
	{
		if (curl)
			return;

		curl_global_init(CURL_GLOBAL_ALL);
		curl = curl_easy_init();
		if (curl)
		{
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curl_slist_append(nullptr, "content-Type: application/json"));
		}
	}

	~API()
	{
		curl_global_cleanup();
		curl_easy_cleanup(curl);
	}

	FORCEINLINE bool Request(EReqType RequestType, const std::string& Endpoint, const std::string& Body, std::string* OutResponse = nullptr)
	{
		curl_easy_setopt(curl, CURLOPT_URL, Endpoint.c_str());
		if (RequestType == EReqType_DELETE)
		{
			curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
		}

		if (!Body.empty())
		{
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, Body.c_str());
		}

		std::string callback;
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Write_Callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &callback);

		CURLcode Res = curl_easy_perform(curl);
		if (OutResponse)
			*OutResponse = callback;

		return Res == CURLE_OK;
	}
};

namespace Backend
{
	static API* api = nullptr;

	void Setup() // Call this at the start of the server
	{
		if (!api)
			api = new API();
	}

	static int ArenaPlacementPoints(int Placement)
	{
		if (Globals::bArenaDuos)
		{
			if (Placement <= 3) return 7;
			if (Placement <= 7) return 5;
			if (Placement <= 12) return 3;
			return 0;
		}
		if (Placement <= 5) return 7;
		if (Placement <= 15) return 5;
		if (Placement <= 25) return 3;
		return 0;
	}

	void ReportArenaPlacement(const std::string& Username, int Placement)
	{
		if (!api || (!Globals::bArenaSolo && !Globals::bArenaDuos) || Username.empty() || Placement < 2)
			return;

		int Points = ArenaPlacementPoints(Placement);
		if (Points <= 0)
			return;

		std::string Encoded;
		for (char c : Username)
			Encoded += (c == ' ') ? std::string("%20") : std::string(1, c);

		std::string Url = Globals::BackendHost + "/volcano/api/vbucks?apikey=" + Globals::VbucksApiKey + "&username=" + Encoded + "&reason=hype&amount=" + std::to_string(Points);
		api->Request(EReqType_GET, Url, "");
		LOG_("arena placement {} -> +{} hype for {}", Placement, Points, Username);
	}

	std::string GetQueuedPlaylist()
	{
		if (!api)
			return "";

		std::string response;
		api->Request(EReqType_GET, Globals::BackendHost + "/gs/playlist", "", &response);

		auto key = response.find("\"playlist\"");
		if (key == std::string::npos)
			return "";
		auto q1 = response.find('"', response.find(':', key));
		if (q1 == std::string::npos)
			return "";
		auto q2 = response.find('"', q1 + 1);
		if (q2 == std::string::npos || q2 <= q1 + 1)
			return "";

		std::string Raw = response.substr(q1 + 1, q2 - q1 - 1);
		if (Raw.find("showdownalt_duos") != std::string::npos) return "Playlist_ShowdownAlt_Duos";
		if (Raw.find("showdownalt_solo") != std::string::npos) return "Playlist_ShowdownAlt_Solo";
		if (Raw.find("defaultduo") != std::string::npos) return "Playlist_DefaultDuo";
		if (Raw.find("defaultsquad") != std::string::npos) return "Playlist_DefaultSquad";
		if (Raw.find("playground") != std::string::npos) return "Playlist_Playground";
		if (Raw.find("50v50") != std::string::npos) return "Playlist_50v50";
		if (Raw.find("solo") != std::string::npos) return "Playlist_DefaultSolo";
		return Raw;
	}

	// You must call Backend::Setup() before doing anything
	void Example()
	{
		if (!api) // You didn't call the Setup Function
			return;

		// your body, if you don't want a body you can just let it ""
		auto Body = "{\"SessionId\": \"YAYAYADZADZAD_Session_Id_Example\", \"Port\": \"7777\"}";

		// the response you will get 
		std::string response;
		api->Request(EReqType_GET, "http://gay.real.ong/matchmaker/idk/stuff/", Body, &response);

		// if it is a JSON response goodluck with jsoning on c++ LOL
	}

	// this is an example of a DELETE request
	void Example_DELETE()
	{
		if (!api) 
			return;

		// your body, if you don't want a body you can just let it ""
		auto Body = "{\"SessionId\": \"YAYAYADZADZAD_Session_Id_Example\", \"Port\": \"7777\"}";

		api->Request(EReqType_DELETE, "http://gay.real.ong/matchmaker/idk/stuff/", Body);

		// if it is a JSON response goodluck with jsoning on c++ LOL
	}
}