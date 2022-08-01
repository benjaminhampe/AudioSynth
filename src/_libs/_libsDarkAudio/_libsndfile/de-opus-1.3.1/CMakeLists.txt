set(LIB_NAME de_opus) # We dont like to clash with system libs.
project(lib-${LIB_NAME}-1.3.1-dev C ASM_NASM)

if (TARGET ${LIB_NAME})
   message("${BoldYellow}[benni] HEADER_GUARD: ${LIB_NAME}${ColourReset}")
else()
   set(OPUS_HEADER_DIR ${CMAKE_CURRENT_LIST_DIR}/include/opus)
   set(OPUS_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
   set(OPUS_HEADERS
      ${OPUS_HEADER_DIR}/opus.h
      ${OPUS_HEADER_DIR}/opus_custom.h
      ${OPUS_HEADER_DIR}/opus_defines.h
      ${OPUS_HEADER_DIR}/opus_multistream.h
      ${OPUS_HEADER_DIR}/opus_projection.h
      ${OPUS_HEADER_DIR}/opus_types.h
   )
   set(OPUS_SOURCES
      ${OPUS_SOURCE_DIR}/config.h
      ${OPUS_SOURCE_DIR}/analysis.c
      ${OPUS_SOURCE_DIR}/analysis.h
      ${OPUS_SOURCE_DIR}/mapping_matrix.c
      ${OPUS_SOURCE_DIR}/mapping_matrix.h
      ${OPUS_SOURCE_DIR}/mlp.c
      ${OPUS_SOURCE_DIR}/mlp.h
      ${OPUS_SOURCE_DIR}/mlp_data.c
      ${OPUS_SOURCE_DIR}/opus.c
      # ${OPUS_SOURCE_DIR}/opus_compare.c # app
      ${OPUS_SOURCE_DIR}/opus_decoder.c
      # ${OPUS_SOURCE_DIR}/opus_demo.c # app
      ${OPUS_SOURCE_DIR}/opus_encoder.c
      ${OPUS_SOURCE_DIR}/opus_multistream.c
      ${OPUS_SOURCE_DIR}/opus_multistream_decoder.c
      ${OPUS_SOURCE_DIR}/opus_multistream_encoder.c
      ${OPUS_SOURCE_DIR}/opus_private.h
      ${OPUS_SOURCE_DIR}/opus_projection_decoder.c
      ${OPUS_SOURCE_DIR}/opus_projection_encoder.c
      ${OPUS_SOURCE_DIR}/repacketizer.c
      # ${OPUS_SOURCE_DIR}/repacketizer_demo.c # app
      ${OPUS_SOURCE_DIR}/tansig_table.h
      # ${OPUS_SOURCE_DIR}/celt/
      ${OPUS_SOURCE_DIR}/celt/arch.h
      # ${OPUS_SOURCE_DIR}/celt/arm/
      ${OPUS_SOURCE_DIR}/celt/bands.c
      ${OPUS_SOURCE_DIR}/celt/bands.h
      ${OPUS_SOURCE_DIR}/celt/celt.c
      ${OPUS_SOURCE_DIR}/celt/celt.h
      ${OPUS_SOURCE_DIR}/celt/celt_decoder.c
      ${OPUS_SOURCE_DIR}/celt/celt_encoder.c
      ${OPUS_SOURCE_DIR}/celt/celt_lpc.c
      ${OPUS_SOURCE_DIR}/celt/celt_lpc.h
      ${OPUS_SOURCE_DIR}/celt/cpu_support.h
      ${OPUS_SOURCE_DIR}/celt/cwrs.c
      ${OPUS_SOURCE_DIR}/celt/cwrs.h
      ${OPUS_SOURCE_DIR}/celt/ecintrin.h
      ${OPUS_SOURCE_DIR}/celt/entcode.c
      ${OPUS_SOURCE_DIR}/celt/entcode.h
      ${OPUS_SOURCE_DIR}/celt/entdec.c
      ${OPUS_SOURCE_DIR}/celt/entdec.h
      ${OPUS_SOURCE_DIR}/celt/entenc.c
      ${OPUS_SOURCE_DIR}/celt/entenc.h
      ${OPUS_SOURCE_DIR}/celt/fixed_debug.h
      ${OPUS_SOURCE_DIR}/celt/fixed_generic.h
      ${OPUS_SOURCE_DIR}/celt/float_cast.h
      ${OPUS_SOURCE_DIR}/celt/kiss_fft.c
      ${OPUS_SOURCE_DIR}/celt/kiss_fft.h
      ${OPUS_SOURCE_DIR}/celt/laplace.c
      ${OPUS_SOURCE_DIR}/celt/laplace.h
      ${OPUS_SOURCE_DIR}/celt/mathops.c
      ${OPUS_SOURCE_DIR}/celt/mathops.h
      ${OPUS_SOURCE_DIR}/celt/mdct.c
      ${OPUS_SOURCE_DIR}/celt/mdct.h
      ${OPUS_SOURCE_DIR}/celt/mfrngcod.h
      # ${OPUS_SOURCE_DIR}/celt/mips/
      ${OPUS_SOURCE_DIR}/celt/modes.c
      ${OPUS_SOURCE_DIR}/celt/modes.h
      # ${OPUS_SOURCE_DIR}/celt/opus_custom_demo.c # app
      ${OPUS_SOURCE_DIR}/celt/os_support.h
      ${OPUS_SOURCE_DIR}/celt/pitch.c
      ${OPUS_SOURCE_DIR}/celt/pitch.h
      ${OPUS_SOURCE_DIR}/celt/quant_bands.c
      ${OPUS_SOURCE_DIR}/celt/quant_bands.h
      ${OPUS_SOURCE_DIR}/celt/rate.c
      ${OPUS_SOURCE_DIR}/celt/rate.h
      ${OPUS_SOURCE_DIR}/celt/stack_alloc.h
      ${OPUS_SOURCE_DIR}/celt/static_modes_fixed.h
      ${OPUS_SOURCE_DIR}/celt/static_modes_fixed_arm_ne10.h
      ${OPUS_SOURCE_DIR}/celt/static_modes_float.h
      ${OPUS_SOURCE_DIR}/celt/static_modes_float_arm_ne10.h
      # ${OPUS_SOURCE_DIR}/celt/tests/
      ${OPUS_SOURCE_DIR}/celt/vq.c
      ${OPUS_SOURCE_DIR}/celt/vq.h     
     
      # ${OPUS_SOURCE_DIR}/silk/      
      ${OPUS_SOURCE_DIR}/silk/A2NLSF.c
      ${OPUS_SOURCE_DIR}/silk/ana_filt_bank_1.c
      ${OPUS_SOURCE_DIR}/silk/API.h
      # ${OPUS_SOURCE_DIR}/silk/arm/
      ${OPUS_SOURCE_DIR}/silk/biquad_alt.c
      ${OPUS_SOURCE_DIR}/silk/bwexpander.c
      ${OPUS_SOURCE_DIR}/silk/bwexpander_32.c
      ${OPUS_SOURCE_DIR}/silk/check_control_input.c
      ${OPUS_SOURCE_DIR}/silk/CNG.c
      ${OPUS_SOURCE_DIR}/silk/code_signs.c
      ${OPUS_SOURCE_DIR}/silk/control.h
      ${OPUS_SOURCE_DIR}/silk/control_audio_bandwidth.c
      ${OPUS_SOURCE_DIR}/silk/control_codec.c
      ${OPUS_SOURCE_DIR}/silk/control_SNR.c
      ${OPUS_SOURCE_DIR}/silk/debug.c
      ${OPUS_SOURCE_DIR}/silk/debug.h
      ${OPUS_SOURCE_DIR}/silk/dec_API.c
      ${OPUS_SOURCE_DIR}/silk/decode_core.c
      ${OPUS_SOURCE_DIR}/silk/decode_frame.c
      ${OPUS_SOURCE_DIR}/silk/decode_indices.c
      ${OPUS_SOURCE_DIR}/silk/decode_parameters.c
      ${OPUS_SOURCE_DIR}/silk/decode_pitch.c
      ${OPUS_SOURCE_DIR}/silk/decode_pulses.c
      ${OPUS_SOURCE_DIR}/silk/decoder_set_fs.c
      ${OPUS_SOURCE_DIR}/silk/define.h
      ${OPUS_SOURCE_DIR}/silk/enc_API.c
      ${OPUS_SOURCE_DIR}/silk/encode_indices.c
      ${OPUS_SOURCE_DIR}/silk/encode_pulses.c
      ${OPUS_SOURCE_DIR}/silk/errors.h
      ${OPUS_SOURCE_DIR}/silk/gain_quant.c
      ${OPUS_SOURCE_DIR}/silk/HP_variable_cutoff.c
      ${OPUS_SOURCE_DIR}/silk/init_decoder.c
      ${OPUS_SOURCE_DIR}/silk/init_encoder.c
      ${OPUS_SOURCE_DIR}/silk/Inlines.h
      ${OPUS_SOURCE_DIR}/silk/inner_prod_aligned.c
      ${OPUS_SOURCE_DIR}/silk/interpolate.c
      ${OPUS_SOURCE_DIR}/silk/lin2log.c
      ${OPUS_SOURCE_DIR}/silk/log2lin.c
      ${OPUS_SOURCE_DIR}/silk/LP_variable_cutoff.c
      ${OPUS_SOURCE_DIR}/silk/LPC_analysis_filter.c
      ${OPUS_SOURCE_DIR}/silk/LPC_fit.c
      ${OPUS_SOURCE_DIR}/silk/LPC_inv_pred_gain.c
      ${OPUS_SOURCE_DIR}/silk/MacroCount.h
      ${OPUS_SOURCE_DIR}/silk/MacroDebug.h
      ${OPUS_SOURCE_DIR}/silk/macros.h
      ${OPUS_SOURCE_DIR}/silk/silk_main.h
      # ${OPUS_SOURCE_DIR}/silk/mips/
      ${OPUS_SOURCE_DIR}/silk/NLSF_decode.c
      ${OPUS_SOURCE_DIR}/silk/NLSF_del_dec_quant.c
      ${OPUS_SOURCE_DIR}/silk/NLSF_encode.c
      ${OPUS_SOURCE_DIR}/silk/NLSF_stabilize.c
      ${OPUS_SOURCE_DIR}/silk/NLSF_unpack.c
      ${OPUS_SOURCE_DIR}/silk/NLSF_VQ.c
      ${OPUS_SOURCE_DIR}/silk/NLSF_VQ_weights_laroia.c
      ${OPUS_SOURCE_DIR}/silk/NLSF2A.c
      ${OPUS_SOURCE_DIR}/silk/NSQ.c
      ${OPUS_SOURCE_DIR}/silk/NSQ.h
      ${OPUS_SOURCE_DIR}/silk/NSQ_del_dec.c
      ${OPUS_SOURCE_DIR}/silk/pitch_est_defines.h
      ${OPUS_SOURCE_DIR}/silk/pitch_est_tables.c
      ${OPUS_SOURCE_DIR}/silk/PLC.c
      ${OPUS_SOURCE_DIR}/silk/PLC.h
      ${OPUS_SOURCE_DIR}/silk/process_NLSFs.c
      ${OPUS_SOURCE_DIR}/silk/quant_LTP_gains.c
      ${OPUS_SOURCE_DIR}/silk/resampler.c
      ${OPUS_SOURCE_DIR}/silk/resampler_down2.c
      ${OPUS_SOURCE_DIR}/silk/resampler_down2_3.c
      ${OPUS_SOURCE_DIR}/silk/resampler_private.h
      ${OPUS_SOURCE_DIR}/silk/resampler_private_AR2.c
      ${OPUS_SOURCE_DIR}/silk/resampler_private_down_FIR.c
      ${OPUS_SOURCE_DIR}/silk/resampler_private_IIR_FIR.c
      ${OPUS_SOURCE_DIR}/silk/resampler_private_up2_HQ.c
      ${OPUS_SOURCE_DIR}/silk/resampler_rom.c
      ${OPUS_SOURCE_DIR}/silk/resampler_rom.h
      ${OPUS_SOURCE_DIR}/silk/resampler_structs.h
      ${OPUS_SOURCE_DIR}/silk/shell_coder.c
      ${OPUS_SOURCE_DIR}/silk/sigm_Q15.c
      ${OPUS_SOURCE_DIR}/silk/SigProc_FIX.h
      ${OPUS_SOURCE_DIR}/silk/sort.c
      ${OPUS_SOURCE_DIR}/silk/stereo_decode_pred.c
      ${OPUS_SOURCE_DIR}/silk/stereo_encode_pred.c
      ${OPUS_SOURCE_DIR}/silk/stereo_find_predictor.c
      ${OPUS_SOURCE_DIR}/silk/stereo_LR_to_MS.c
      ${OPUS_SOURCE_DIR}/silk/stereo_MS_to_LR.c
      ${OPUS_SOURCE_DIR}/silk/stereo_quant_pred.c
      ${OPUS_SOURCE_DIR}/silk/structs.h
      ${OPUS_SOURCE_DIR}/silk/sum_sqr_shift.c
      ${OPUS_SOURCE_DIR}/silk/table_LSF_cos.c
      ${OPUS_SOURCE_DIR}/silk/tables.h
      ${OPUS_SOURCE_DIR}/silk/tables_gain.c
      ${OPUS_SOURCE_DIR}/silk/tables_LTP.c
      ${OPUS_SOURCE_DIR}/silk/tables_NLSF_CB_NB_MB.c
      ${OPUS_SOURCE_DIR}/silk/tables_NLSF_CB_WB.c
      ${OPUS_SOURCE_DIR}/silk/tables_other.c
      ${OPUS_SOURCE_DIR}/silk/tables_pitch_lag.c
      ${OPUS_SOURCE_DIR}/silk/tables_pulses_per_block.c
      # silk/tests/
      ${OPUS_SOURCE_DIR}/silk/tuning_parameters.h
      ${OPUS_SOURCE_DIR}/silk/typedef.h
      ${OPUS_SOURCE_DIR}/silk/VAD.c
      ${OPUS_SOURCE_DIR}/silk/VQ_WMat_EC.c
      # silk/fixed/
      # ${OPUS_SOURCE_DIR}/silk/fixed/apply_sine_window_FIX.c
      # silk/fixed/arm/
      # ${OPUS_SOURCE_DIR}/silk/fixed/autocorr_FIX.c
      # # ${OPUS_SOURCE_DIR}/silk/fixed/burg_modified_FIX.c
      # ${OPUS_SOURCE_DIR}/silk/fixed/corrMatrix_FIX.c
      # ${OPUS_SOURCE_DIR}/silk/fixed/encode_frame_FIX.c
      # ${OPUS_SOURCE_DIR}/silk/fixed/find_LPC_FIX.c
      # ${OPUS_SOURCE_DIR}/silk/fixed/find_LTP_FIX.c
      # ${OPUS_SOURCE_DIR}/silk/fixed/find_pitch_lags_FIX.c
      # ${OPUS_SOURCE_DIR}/silk/fixed/find_pred_coefs_FIX.c
      # ${OPUS_SOURCE_DIR}/silk/fixed/k2a_FIX.c
      # ${OPUS_SOURCE_DIR}/silk/fixed/k2a_Q16_FIX.c
      # ${OPUS_SOURCE_DIR}/silk/fixed/LTP_analysis_filter_FIX.c
      # ${OPUS_SOURCE_DIR}/silk/fixed/LTP_scale_ctrl_FIX.c
      # ${OPUS_SOURCE_DIR}/silk/fixed/main_FIX.h
      # # silk/fixed/mips/
      # ${OPUS_SOURCE_DIR}/silk/fixed/noise_shape_analysis_FIX.c
      # ${OPUS_SOURCE_DIR}/silk/fixed/pitch_analysis_core_FIX.c
      # ${OPUS_SOURCE_DIR}/silk/fixed/process_gains_FIX.c
      # ${OPUS_SOURCE_DIR}/silk/fixed/regularize_correlations_FIX.c
      # ${OPUS_SOURCE_DIR}/silk/fixed/residual_energy_FIX.c
      # ${OPUS_SOURCE_DIR}/silk/fixed/residual_energy16_FIX.c
      # ${OPUS_SOURCE_DIR}/silk/fixed/schur_FIX.c
      # ${OPUS_SOURCE_DIR}/silk/fixed/schur64_FIX.c
      # ${OPUS_SOURCE_DIR}/silk/fixed/structs_FIX.h
      # ${OPUS_SOURCE_DIR}/silk/fixed/vector_ops_FIX.c
      # ${OPUS_SOURCE_DIR}/silk/fixed/warped_autocorrelation_FIX.c
      #${OPUS_SOURCE_DIR}/silk/float/
      ${OPUS_SOURCE_DIR}/silk/float/apply_sine_window_FLP.c
      ${OPUS_SOURCE_DIR}/silk/float/autocorrelation_FLP.c
      ${OPUS_SOURCE_DIR}/silk/float/burg_modified_FLP.c
      ${OPUS_SOURCE_DIR}/silk/float/bwexpander_FLP.c
      ${OPUS_SOURCE_DIR}/silk/float/corrMatrix_FLP.c
      ${OPUS_SOURCE_DIR}/silk/float/encode_frame_FLP.c
      ${OPUS_SOURCE_DIR}/silk/float/energy_FLP.c
      ${OPUS_SOURCE_DIR}/silk/float/find_LPC_FLP.c
      ${OPUS_SOURCE_DIR}/silk/float/find_LTP_FLP.c
      ${OPUS_SOURCE_DIR}/silk/float/find_pitch_lags_FLP.c
      ${OPUS_SOURCE_DIR}/silk/float/find_pred_coefs_FLP.c
      ${OPUS_SOURCE_DIR}/silk/float/inner_product_FLP.c
      ${OPUS_SOURCE_DIR}/silk/float/k2a_FLP.c
      ${OPUS_SOURCE_DIR}/silk/float/LPC_analysis_filter_FLP.c
      ${OPUS_SOURCE_DIR}/silk/float/LPC_inv_pred_gain_FLP.c
      ${OPUS_SOURCE_DIR}/silk/float/LTP_analysis_filter_FLP.c
      ${OPUS_SOURCE_DIR}/silk/float/LTP_scale_ctrl_FLP.c
      ${OPUS_SOURCE_DIR}/silk/float/main_FLP.h
      ${OPUS_SOURCE_DIR}/silk/float/noise_shape_analysis_FLP.c
      ${OPUS_SOURCE_DIR}/silk/float/pitch_analysis_core_FLP.c
      ${OPUS_SOURCE_DIR}/silk/float/process_gains_FLP.c
      ${OPUS_SOURCE_DIR}/silk/float/regularize_correlations_FLP.c
      ${OPUS_SOURCE_DIR}/silk/float/residual_energy_FLP.c
      ${OPUS_SOURCE_DIR}/silk/float/scale_copy_vector_FLP.c
      ${OPUS_SOURCE_DIR}/silk/float/scale_vector_FLP.c
      ${OPUS_SOURCE_DIR}/silk/float/schur_FLP.c
      ${OPUS_SOURCE_DIR}/silk/float/SigProc_FLP.h
      ${OPUS_SOURCE_DIR}/silk/float/sort_FLP.c
      ${OPUS_SOURCE_DIR}/silk/float/structs_FLP.h
      ${OPUS_SOURCE_DIR}/silk/float/warped_autocorrelation_FLP.c
      ${OPUS_SOURCE_DIR}/silk/float/wrappers_FLP.c
   )

   
   list(APPEND OPUS_SOURCES
      ${OPUS_SOURCE_DIR}/celt/x86/x86_celt_map.c
      ${OPUS_SOURCE_DIR}/celt/x86/x86cpu.c
      ${OPUS_SOURCE_DIR}/celt/x86/x86cpu.h
      ${OPUS_SOURCE_DIR}/silk/x86/main_sse.h
      # ${OPUS_SOURCE_DIR}/silk/x86/NSQ_del_dec_sse4_1.c
      # ${OPUS_SOURCE_DIR}/silk/x86/NSQ_sse4_1.c
      ${OPUS_SOURCE_DIR}/silk/x86/SigProc_FIX_sse.h
      # ${OPUS_SOURCE_DIR}/silk/x86/VAD_sse4_1.c
      # ${OPUS_SOURCE_DIR}/silk/x86/VQ_WMat_EC_sse4_1.c
      ${OPUS_SOURCE_DIR}/silk/x86/x86_silk_map.c      
      ${OPUS_SOURCE_DIR}/celt/x86/vq_sse.h
      ${OPUS_SOURCE_DIR}/celt/x86/vq_sse2.c
      ${OPUS_SOURCE_DIR}/celt/x86/pitch_sse.c
      ${OPUS_SOURCE_DIR}/celt/x86/pitch_sse.h
      # ${OPUS_SOURCE_DIR}/celt/x86/celt_lpc_sse.h
      # ${OPUS_SOURCE_DIR}/celt/x86/celt_lpc_sse4_1.c
      ${OPUS_SOURCE_DIR}/celt/x86/pitch_sse2.c
      # ${OPUS_SOURCE_DIR}/celt/x86/pitch_sse4_1.c      
      # ${OPUS_SOURCE_DIR}/silk/fixed/x86/burg_modified_FIX_sse4_1.c
      # ${OPUS_SOURCE_DIR}/silk/fixed/x86/vector_ops_FIX_sse4_1.c
   )
   
   # if (USE_NASM)
      # target_compile_definitions(${LIB_NAME} PUBLIC -DHAVE_MMX=1)   
      # enable_language(NASM)
      # # common/x86
      # list(APPEND OPUS_SOURCES
         # ${OPUS_SOURCE_DIR}/common/x86/bitstream.h
      # )
   # endif ()

   # if (BUILD_SHARED)
      # message("${BoldBlue}[benni] SHARED_LIBRARY: ${LIB_NAME}${ColourReset}")
      # add_library(${LIB_NAME} SHARED "")
   # else()
      message("${BoldBlue}[benni] STATIC_LIBRARY: ${LIB_NAME}${ColourReset}")
      add_library(${LIB_NAME} STATIC "")
   # endif()

   
   target_compile_definitions(${LIB_NAME} PUBLIC -DUSE_LIB_OPUS)
   target_compile_definitions(${LIB_NAME} PRIVATE -DHAVE_CONFIG_H=1)

   target_sources(${LIB_NAME} PUBLIC ${OPUS_HEADERS} PRIVATE ${OPUS_SOURCES})
   
   target_include_directories(${LIB_NAME} PUBLIC ${OPUS_HEADER_DIR}/..)
   target_include_directories(${LIB_NAME} PRIVATE ${OPUS_HEADER_DIR})
   target_include_directories(${LIB_NAME} PRIVATE ${OPUS_SOURCE_DIR})
   target_include_directories(${LIB_NAME} PRIVATE ${OPUS_SOURCE_DIR}/celt)
   target_include_directories(${LIB_NAME} PRIVATE ${OPUS_SOURCE_DIR}/silk)
   target_include_directories(${LIB_NAME} PRIVATE ${OPUS_SOURCE_DIR}/silk/fixed)
   target_include_directories(${LIB_NAME} PRIVATE ${OPUS_SOURCE_DIR}/silk/float)

   target_link_libraries(${LIB_NAME} PUBLIC pthread m)

endif()

# add_subdirectory(test)